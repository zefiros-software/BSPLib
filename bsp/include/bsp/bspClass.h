#pragma once
#ifndef __BSPCLASS_H__
#define __BSPCLASS_H__

#define BSP_SKIP_CHECKS

#include "bsp/communicationQueues.h"
#include "bsp/condVarBarrier.h"
#include "bsp/mixedBarrier.h"
#include "bsp/requests.h"
#include "bsp/barrier.h"

#include <unordered_map>
#include <assert.h>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <future>

extern int main( int argc, char **argv );

class BSP
{
public:

    BSP_FORCEINLINE void VAbort( const char *format, va_list args )
    {
        mAbort = true;
        vfprintf( stderr, format, args );
        CheckAborted();
    }

    inline void Abort( const char *format, ... )
    {
        va_list args;
        va_start( args, format );

        //pass to bsp_vabort
        VAbort( format, args );

        //mark end of variable arguments
        va_end( args );
    }

    BSP_FORCEINLINE size_t NProcs() const
    {
        return mProcCount > 0 ? mProcCount : std::thread::hardware_concurrency();
    }

    BSP_FORCEINLINE void QSize( size_t *packets, size_t *accumulated_size )
    {
        *packets = 0;

        if ( accumulated_size )
        {
            *accumulated_size = 0;
        }

        std::vector< BspInternal::SendRequest > &sendQueue = mSendRequests[ProcId()];
        *packets += sendQueue.size();

        if ( accumulated_size )
        {
            for ( const auto &request : sendQueue )
            {
                *accumulated_size += request.bufferSize;
            }
        }
    }

    BSP_FORCEINLINE size_t &ProcId()
    {
        static thread_local size_t gPID = 0xdeadbeef;

        return gPID;
    }

    BSP_FORCEINLINE double Time()
    {
        CheckAborted();

        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mStartTimes[ProcId()];
        return diff.count();
    }

    BSP_FORCEINLINE void Init( std::function< void() > entry, uint32_t, char *[] )
    {
        mEntry = entry;
        mTagSize = 0;

        if ( !mEnded )
        {
            fprintf( stderr, "Warning: initialisation data corresponding to another BSP run found;\n" );
            fprintf( stderr, "         and this other run did not terminate (gracefully).\n" );
        }

        ProcId() = 0;
    }

    void Begin( uint32_t maxProcs )
    {
        //fprintf( stderr, "Begin %ld-%ld\n", PID(), std::this_thread::get_id() );

        if ( ProcId() )
        {

#ifndef BSP_SKIP_CHECKS
            assert( maxProcs == mProcCount );
            assert( ProcId() < maxProcs );
#endif // SKIP_CHECKS

            StartTiming();
            return;
        }

        mAbort = false;
        mEnded = false;
        mProcCount = maxProcs;

        mRegisters.clear();
        mRegisters.resize( maxProcs );

        mRegisterCount.clear();
        mRegisterCount.resize( maxProcs );

        mThreadRegisterLocation.clear();
        mThreadRegisterLocation.resize( maxProcs );

        mPutRequests.ResetResize( maxProcs );

        mGetRequests.ResetResize( maxProcs );

        mTmpSendRequests.ResetResize( maxProcs );
        mTmpSendBuffers.ResetResize( maxProcs );

        mSendRequests.clear();
        mSendRequests.resize( maxProcs );
        mSendBuffers.clear();
        mSendBuffers.resize( maxProcs );

        mPushRequests.clear();
        mPushRequests.resize( maxProcs );

        mPopRequests.clear();
        mPopRequests.resize( maxProcs );

        mThreadBarrier.SetSize( maxProcs );

        mPutBufferStacks.clear();
        mPutBufferStacks.resize( maxProcs, 9064 );

        mNewTagSize.clear();
        mNewTagSize.resize( maxProcs );

        mSendReceivedIndex.clear();
        mSendReceivedIndex.resize( maxProcs );

        mStartTimes.clear();
        mStartTimes.resize( maxProcs );
        mThreads.reserve( maxProcs );

        for ( uint32_t i = 1; i < mProcCount; ++i )
        {
            mThreads.emplace_back( std::async( std::launch::async, [this]( uint32_t pid )
            {
                ProcId() = pid;

                try
                {
                    if ( mEntry )
                    {
                        mEntry();
                    }
                    else
                    {
                        main( 0, nullptr );
                    }

                }
                catch ( BspInternal::BspAbort & )
                {
                    // pass
                }
            }, i ) );
        }

        StartTiming();
    }

    void End()
    {
        mEnded = true;

        SyncPoint();

        if ( ProcId() == 0 )
        {
            mThreads.clear();
        }
    }

    BSP_FORCEINLINE void Sync()
    {
        size_t &pid = ProcId();

        SyncPoint();

        if ( pid == 0 && mNewTagSize[0] != mTagSize )
        {
            mTagSize = mNewTagSize[0];
        }

        ProcessGetRequests( pid );

        SyncPoint();

        ProcessPopRequests( pid );

        ProcessSendRequests( pid );

        ProcessPutRequests( pid );

        SyncPoint();

        mPutBufferStacks[pid].Clear();

        ProcessPushRequests( pid );

        SyncPoint();
    }

    BSP_FORCEINLINE void PushReg( const void *ident, size_t size )
    {
        size_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mRegisters.size() > pid );
        assert( mRegisterCount.size() > pid );
        assert( mThreadRegisterLocation.size() > pid );
#endif

        mPushRequests[pid].emplace_back( BspInternal::PushRequest{ ident, { size, mRegisterCount[pid]++ } } );
    }

    void PopReg( const void *ident )
    {
        size_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mRegisters.size() > pid );
        assert( mRegisterCount.size() > pid );
        assert( mThreadRegisterLocation.size() > pid );
#endif

        mPopRequests[pid].emplace_back( BspInternal::PopRequest{ ident } );
    }

    BSP_FORCEINLINE void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {
        size_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( tpid < mProcCount );
        assert( pid < mProcCount );
#endif

        const char *srcBuff = reinterpret_cast<const char *>( src );
        const size_t globalId = mRegisters[tpid][dst].registerCount;

#ifndef BSP_SKIP_CHECKS
        assert( mThreadRegisterLocation[pid].size() > globalId );
        assert( mRegisters[pid][mThreadRegisterLocation[tpid][globalId]].size >= nbytes );
#endif

        const char *dstBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );
        BspInternal::StackAllocator::StackLocation bufferLocation = mPutBufferStacks[tpid].Alloc( nbytes, srcBuff );

        mPutRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::PutRequest{ bufferLocation, dstBuff + offset, nbytes } );
    }

    BSP_FORCEINLINE void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {
        size_t &tpid = ProcId();

        assert( pid < mProcCount );
        assert( tpid < mProcCount );

        const size_t globalId = mRegisters[tpid][src].registerCount;

        assert( mRegisters[pid][mThreadRegisterLocation[pid][globalId]].size >= nbytes );

        const char *srcBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );

        mGetRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::GetRequest{ dst, srcBuff + offset, nbytes } );
    }

    BSP_FORCEINLINE void Send( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        size_t &tpid = ProcId();

        assert( pid < mProcCount );
        assert( tpid < mProcCount );
        assert( mNewTagSize[tpid] == mTagSize );

        const char *srcBuff = reinterpret_cast<const char *>( payload );
        const char *tagBuff = reinterpret_cast<const char *>( tag );

        BspInternal::StackAllocator &tmpSendBuffer = mTmpSendBuffers.GetQueueFromMe( pid, tpid );

        BspInternal::StackAllocator::StackLocation bufferLocation = tmpSendBuffer.Alloc( size, srcBuff );
        BspInternal::StackAllocator::StackLocation tagLocation = tmpSendBuffer.Alloc( mTagSize, tagBuff );

        mTmpSendRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::SendRequest{ bufferLocation, size, tagLocation, mTagSize } );
    }

    BSP_FORCEINLINE void Move( void *payload, size_t max_copy_size_in )
    {
        size_t &pid = ProcId();

        if ( mSendRequests[pid].empty() )
        {
            return;
        }

        BspInternal::SendRequest &request = mSendRequests[pid][mSendReceivedIndex[pid]++];

        const size_t copySize = std::min( max_copy_size_in, request.bufferSize );
        mSendBuffers[pid].Extract( request.bufferLocation, copySize, ( char * )payload );
    }

    BSP_FORCEINLINE void SetTagsize( size_t *size )
    {
        const size_t newSize = *size;
        *size = mTagSize;
        mNewTagSize[ProcId()] = newSize;
    }

    BSP_FORCEINLINE void GetTag( size_t *status, void *tag )
    {
        size_t &pid = ProcId();
        *status = ( size_t ) - 1;

        size_t index = mSendReceivedIndex[pid];
        std::vector< BspInternal::SendRequest > &sendQueue = mSendRequests[pid];

        if ( !sendQueue.empty() && index < sendQueue.size() )
        {
            BspInternal::SendRequest &sendRequest = sendQueue[index];
            *status = sendRequest.bufferSize;

            char *tagBuff = reinterpret_cast<char *>( tag );

            assert( sendQueue[index].tagSize == mTagSize );

            mSendBuffers[pid].Extract( sendRequest.tagLocation, sendRequest.tagSize, tagBuff );
        }
    }

    BSP_FORCEINLINE bool IsEnded() const
    {
        return mEnded;
    }

    static BSP_FORCEINLINE BSP &GetInstance()
    {
        static BSP mBSP;
        return mBSP;
    }

private:

    BspInternal::MixedBarrier mThreadBarrier;
    std::vector< BspInternal::StackAllocator > mPutBufferStacks;

    BspInternal::CommunicationQueues< std::vector< BspInternal::PutRequest > > mPutRequests;
    BspInternal::CommunicationQueues< std::vector< BspInternal::GetRequest > > mGetRequests;

    BspInternal::CommunicationQueues< std::vector< BspInternal::SendRequest > > mTmpSendRequests;
    BspInternal::CommunicationQueues< BspInternal::StackAllocator > mTmpSendBuffers;
    std::vector< BspInternal::StackAllocator > mSendBuffers;
    std::vector< std::vector< BspInternal::SendRequest > > mSendRequests;

    std::vector< size_t > mSendReceivedIndex;

    std::vector< std::vector< BspInternal::PushRequest > > mPushRequests;
    std::vector< std::vector< BspInternal::PopRequest > > mPopRequests;

    std::vector< size_t > mRegisterCount;
    std::vector< std::map< const void *, BspInternal::RegisterInfo > > mRegisters;
    std::vector< std::vector< const void * > > mThreadRegisterLocation;

    std::vector< std::future< void > > mThreads;
    std::function< void() > mEntry;
    std::vector< std::chrono::time_point< std::chrono::high_resolution_clock > > mStartTimes;
    size_t mProcCount;
    std::vector<size_t> mNewTagSize;
    std::atomic_size_t mTagSize;

    bool mEnded;
    std::atomic_bool mAbort;

    BSP()
        : mThreadBarrier( 0 ),
          mPutBufferStacks( 0 ),
          mProcCount( 0 ),
          mTagSize( 0 ),
          mEnded( true ),
          mAbort( false )
    {
    }

    void StartTiming()
    {
        mStartTimes[ProcId()] = std::chrono::high_resolution_clock::now();
    }

    void SyncPoint()
    {
        mThreadBarrier.Wait( mAbort );
    }

    void CheckAborted() const
    {
        if ( mAbort )
        {
            std::terminate();
        }
    }

    BSP_FORCEINLINE void ProcessPushRequests( size_t pid )
    {
        if ( !mPushRequests[pid].empty() )
        {
            for ( const auto &pushRequest : mPushRequests[pid] )
            {
                mRegisters[pid][pushRequest.pushRegister] = pushRequest.registerInfo;
                mThreadRegisterLocation[pid].push_back( pushRequest.pushRegister );
            }

            mPushRequests[pid].clear();
        }
    }

    BSP_FORCEINLINE void ProcessPutRequests( size_t pid )
    {
        for ( size_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::PutRequest > &putQueue = mPutRequests.GetQueueToMe( owner, pid );

            if ( !putQueue.empty() )
            {
                for ( auto putRequest = putQueue.rbegin(), end = putQueue.rend(); putRequest != end; ++putRequest )
                {
#ifndef BSP_SKIP_CHECKS
                    assert( putRequest->size > 0 );
#endif
                    mPutBufferStacks[owner].Extract( putRequest->bufferLocation, putRequest->size, ( char * )putRequest->destination );
                }

                putQueue.clear();
            }
        }
    }

    BSP_FORCEINLINE void ProcessSendRequests( size_t pid )
    {
        mSendRequests[pid].clear();
        mSendReceivedIndex[pid] = 0;

        BspInternal::StackAllocator::StackLocation offset = 0;
        BspInternal::StackAllocator &sendBuffer = mSendBuffers[pid];

        sendBuffer.Clear();

        for ( size_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::SendRequest > &tmpQueue = mTmpSendRequests.GetQueueToMe( owner, pid );

            if ( !tmpQueue.empty() )
            {
                for ( auto &sendRequest : tmpQueue )
                {
                    sendRequest.bufferLocation += offset;
                    sendRequest.tagLocation += offset;
                }

                std::copy( std::make_move_iterator( tmpQueue.begin() ), std::make_move_iterator( tmpQueue.end() ),
                           std::back_insert_iterator< std::vector< BspInternal::SendRequest > >( mSendRequests[pid] ) );
                tmpQueue = std::vector< BspInternal::SendRequest >();

                BspInternal::StackAllocator &tmpBuffer = mTmpSendBuffers.GetQueueToMe( owner, pid );

                offset += tmpBuffer.Size();
                sendBuffer.MoveBack( tmpBuffer );
            }
        }
    }

    BSP_FORCEINLINE void ProcessPopRequests( size_t pid )
    {
        if ( !mPopRequests[pid].empty() )
        {
            for ( const auto &popRequest : mPopRequests[pid] )
            {
                mRegisters[pid].erase( popRequest.popRegister );
            }

            mPopRequests[pid].clear();
        }
    }

    BSP_FORCEINLINE void ProcessGetRequests( size_t pid )
    {
        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::GetRequest > &getQueue = mGetRequests.GetQueueToMe( owner, pid );

            for ( auto request = getQueue.rbegin(), end = getQueue.rend(); request != end; ++request )
            {
                const char *srcBuff = reinterpret_cast<const char *>( request->source );

                BspInternal::StackAllocator::StackLocation bufferLocation = mPutBufferStacks[pid].Alloc( request->size, srcBuff );

                mPutRequests.GetQueueFromMe( owner, pid ).emplace_back( BspInternal::PutRequest{ bufferLocation, request->destination, request->size } );
            }

            getQueue.clear();
        }
    }

};

#endif