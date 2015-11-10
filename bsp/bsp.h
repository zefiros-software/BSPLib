#pragma once
#ifndef __BSP_H__
#define __BSP_H__

#include <condition_variable>
#include <unordered_map>
#include <assert.h>
#include <thread>
#include <future>
#include <mutex>
#include <stdarg.h>
#include <cstring>
#include <vector>
#include <exception>
#include <iterator>
#include <cstddef>

#define SKIP_CHECKS

thread_local static size_t gPID = 0;

extern int main( int argc, char **argv );

class BSP
{
public:

    BSP()
        : mThreadBarrier( 0 ),
          mThreadBuffers( 0 ),
          mProcCount( 0 ),
          mTagSize( 0 ),
          mEnded( true ),
          mAbort( false )
    {
    }

    void VAbort( const char *format, va_list args )
    {
        mAbort = true;
        vfprintf( stderr, format, args );
        CheckAborted();
    }

    void Abort( const char *format, ... )
    {
        va_list args;
        va_start( args, format );

        //pass to bsp_vabort
        VAbort( format, args );

        //mark end of variable arguments
        va_end( args );
    }

    size_t NProcs() const
    {
        return mProcCount > 0 ? mProcCount : std::thread::hardware_concurrency();
    }

    void QSize( size_t *packets, size_t *accumulated_size )
    {
        *packets = 0;

        if ( accumulated_size )
        {
            *accumulated_size = 0;
        }

        std::vector< SendRequest > &sendQueue = mSendRequests[gPID];
        *packets += sendQueue.size();

        if ( accumulated_size )
        {
            for ( const auto &request : sendQueue )
            {
                *accumulated_size += request.buffer.size();
            }
        }
    }

    static size_t PID()
    {
        return gPID;
    }

    double Time() const
    {
        CheckAborted();

        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mStartTimes[gPID];
        return diff.count();
    }

    void Init( std::function< void() > entry, uint32_t argc, char *argv[] )
    {
        mEntry = entry;
        mTagSize = 0;

        if ( !mEnded )
        {
            fprintf( stderr, "Warning: initialisation data corresponding to another BSP run found;\n" );
            fprintf( stderr, "         and this other run did not terminate (gracefully).\n" );
        }
    }

    void Begin( size_t maxProcs )
    {
        if ( gPID )
        {
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

        mSendRequests.clear();
        mSendRequests.resize( maxProcs );

        mPushRequests.clear();
        mPushRequests.resize( maxProcs );

        mPopRequests.clear();
        mPopRequests.resize( maxProcs );

        mThreadBarrier.SetSize( maxProcs );
        mThreadBuffers.SetSize( maxProcs );

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
                gPID = pid;

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
                catch ( BSPAbort & )
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

        if ( gPID == 0 )
        {
            mThreads.clear();
        }
        else
        {
            //std::terminate();
        }
    }

    void Sync()
    {
        SyncPoint();

        if ( gPID == 0 && mNewTagSize[ 0 ] != mTagSize )
        {
            mTagSize = mNewTagSize[ 0 ];
        }

        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< GetRequest > &getQueue = mGetRequests.GetQueueToMe( owner );

            for ( auto request = getQueue.rbegin(), end = getQueue.rend(); request != end; ++request )
            {
                const char *srcBuff = reinterpret_cast<const char *>( request->source );

                std::vector< char > *buffer = mThreadBuffers.Aquire( request->size );
                memcpy( buffer->data(), srcBuff, request->size );

                mPutRequests.GetQueueFromMe( owner ).emplace_back( PutRequest{ buffer, request->destination } );
            }

            getQueue.clear();
        }

        SyncPoint();

        for ( const auto &popRequest : mPopRequests[ gPID ] )
        {
            mRegisters[gPID].erase( popRequest.popRegister );
        }

        mPopRequests[gPID].clear();

        assert( mTagSize == mNewTagSize[gPID] );

        mSendRequests[gPID].clear();
        mSendReceivedIndex[gPID] = 0;

        for ( size_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< SendRequest > &tmpQueue = mTmpSendRequests.GetQueueToMe( owner );

            if ( !tmpQueue.empty() )
            {
                std::copy( std::make_move_iterator( tmpQueue.begin() ), std::make_move_iterator( tmpQueue.end() ),
                           std::back_insert_iterator< std::vector< SendRequest > >( mSendRequests[gPID] ) );
                tmpQueue = std::vector< SendRequest >();
            }
        }

        for ( size_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< PutRequest > &putQueue = mPutRequests.GetQueueToMe( owner );

            for ( auto putRequest = putQueue.rbegin(), end = putQueue.rend(); putRequest != end; ++putRequest )
            {
                std::vector< char > *buffer = putRequest->buffer;

#ifndef SKIP_CHECKS
                assert( buffer->size() > 0 );
                {
                    char *dstCheck = ( char * )mThreadRegisterLocation[gPID][putRequest->globalId];
                    assert( dstCheck + putRequest->offset == putRequest->destination );
                    assert( buffer->size() + putRequest->offset <= mRegisters[gPID][dstCheck].size );
                }
#endif

                memcpy( ( char * )putRequest->destination, buffer->data(), buffer->size() );

                mThreadBuffers.Release( buffer );
            }

            putQueue.clear();
        }

        SyncPoint();

        for ( const auto &pushRequest : mPushRequests[gPID] )
        {
            mRegisters[gPID][pushRequest.pushRegister] = pushRequest.registerInfo;
            mThreadRegisterLocation[gPID].push_back( pushRequest.pushRegister );
        }

        mPushRequests[gPID].clear();

        SyncPoint();
    }

    void PushReg( const void *ident, size_t size )
    {
        CheckAborted();

#ifndef SKIP_CHECKS
        assert( gPID < mProcCount );
        assert( mRegisters.size() > gPID );
        assert( mRegisterCount.size() > gPID );
        assert( mThreadRegisterLocation.size() > gPID );
#endif

        mPushRequests[ gPID ].push_back( { ident, { size, mRegisterCount[ gPID ]++ } } );
    }

    void PopReg( const void *ident )
    {
        CheckAborted();

#ifndef SKIP_CHECKS
        assert( gPID < mProcCount );
        assert( mRegisters.size() > gPID );
        assert( mRegisterCount.size() > gPID );
        assert( mThreadRegisterLocation.size() > gPID );
#endif

        mPopRequests[ gPID ].push_back( { ident } );
    }

    void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {
#ifndef SKIP_CHECKS
        assert( pid < mProcCount );
        assert( gPID < mProcCount );
#endif

        const char *srcBuff = reinterpret_cast<const char *>( src );
        const size_t globalId = mRegisters[gPID][dst].registerCount;

#ifndef SKIP_CHECKS
        assert( mThreadRegisterLocation[ pid ].size() > globalId );
        assert( mRegisters[pid][mThreadRegisterLocation[pid][globalId]].size >= nbytes );
#endif

        const char *dstBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );
        std::vector< char > *buffer = mThreadBuffers.Aquire( nbytes );
        memcpy( buffer->data(), srcBuff, nbytes );
        mPutRequests.GetQueueFromMe( pid ).emplace_back( PutRequest{ buffer, dstBuff + offset, globalId, offset } );

#ifndef SKIP_CHECKS
        assert( mPutRequests.GetQueueFromMe( pid ).back().buffer->size() == nbytes );
#endif
    }

    void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {
        assert( pid < mProcCount );
        assert( gPID < mProcCount );

        const size_t globalId = mRegisters[gPID][src].registerCount;

        assert( mRegisters[pid][mThreadRegisterLocation[pid][globalId]].size >= nbytes );

        const char *srcBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );

        mGetRequests.GetQueueFromMe( pid ).emplace_back( GetRequest{ dst, srcBuff + offset, nbytes } );
    }

    void Send( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        assert( pid < mProcCount );
        assert( gPID < mProcCount );

        const char *srcBuff = reinterpret_cast<const char *>( payload );
        const char *tagBuff = reinterpret_cast<const char *>( tag );

        mTmpSendRequests.GetQueueFromMe( pid ).emplace_back( SendRequest{ std::vector< char >( srcBuff, srcBuff + size ), std::vector< char >( tagBuff, tagBuff + mNewTagSize[ gPID ] ) } );
    }

    void Move( void *payload, size_t max_copy_size_in )
    {
        if ( mSendRequests[gPID].empty() )
        {
            return;
        }

        SendRequest &request = mSendRequests[gPID][mSendReceivedIndex[gPID]++];

        memcpy( payload, request.buffer.data(), std::min( request.buffer.size(), max_copy_size_in ) );
    }

    void SetTagsize( size_t *size )
    {
        const size_t newSize = *size;
        *size = mTagSize;
        mNewTagSize[ gPID ] = newSize;
    }

    void GetTag( size_t *status, void *tag )
    {
        *status = -1;

        size_t index = mSendReceivedIndex[gPID];
        std::vector< SendRequest > &sendQueue = mSendRequests[gPID];

        if ( !mSendRequests[gPID].empty() && index < sendQueue.size() )
        {
            *status = sendQueue[index].buffer.size();

            char *tagBuff = reinterpret_cast< char * >( tag );

            assert( sendQueue[index].tag.size() == mTagSize );

            memcpy( tagBuff, sendQueue[index].tag.data(), mTagSize );
        }
    }

    bool IsEnded() const
    {
        return mEnded;
    }

    static BSP &GetInstance()
    {
        static BSP mBSP;
        return mBSP;
    }

private:

    class BSPAbort
        : public std::exception
    {
    public:

        BSPAbort( std::string m )
            : msg( m )
        {
        }

        ~BSPAbort() throw()
        {
        }

        const char *what() const throw()
        {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    class Barrier
    {
    public:

        Barrier( uint32_t count_ ) :
            mCount( count_ ),
            mSpaces( count_ ),
            mGeneration( 0 )
        {
        }

        void SetSize( uint32_t count )
        {
            mCount = count;
            mSpaces = count;
            mGeneration = 0;
        }

        __declspec( noinline ) void Wait( std::atomic_bool *aborted )
        {
            const uint32_t myGeneration = mGeneration;

            if ( !--mSpaces )
            {
                mSpaces = mCount;
                ++mGeneration;
            }
            else
            {
                while ( mGeneration == myGeneration )
                {
                    if ( aborted && *aborted )
                    {
                        throw BSPAbort( "Thread Exited" );
                    }
                }
            }
        }

    private:

        uint32_t mCount;
        std::atomic_int_fast32_t mSpaces;
        std::atomic_int_fast32_t mGeneration;
    };

    class SpinningCountBarrier
    {
    public:

        explicit SpinningCountBarrier( size_t count )
            : mEntryCounts( count )
        {
        }

        void SetSize( size_t count )
        {
            mEntryCounts.clear();
            mEntryCounts.resize( count );
        }

        void Wait( std::atomic_bool *aborted )
        {
            size_t myCount = ++mEntryCounts[gPID];

            if ( aborted && *aborted )
            {
                throw BSPAbort( "Thread Exited" );
            }

            for ( size_t &count : mEntryCounts )
            {
                while ( count < myCount )
                {
                    // Besides being useful in breaking the loop, this check forces the count reference to sync
                    if ( aborted && aborted->load() )
                    {
                        break;
                    }
                }
            }

            if ( aborted && *aborted )
            {
                throw BSPAbort( "Thread Exited" );
            }
        }

    private:

        std::vector< size_t > mEntryCounts;
    };


    struct RegisterInfo
    {
        size_t size;
        size_t registerCount;
    };

    class SpinLock
    {
    public:

        SpinLock()
        {
        }

        void lock()
        {
            while ( mLockValue.test_and_set( std::memory_order_acquire ) )
            {
            }
        }

        bool try_lock()
        {
            return !mLockValue.test_and_set( std::memory_order_acquire );
        }

        void unlock()
        {
            mLockValue.clear( std::memory_order_release );
        }

    private:

        std::atomic_flag mLockValue;

        SpinLock( const SpinLock & );
    };

    class CommunicationBuffers
    {
    public:

        CommunicationBuffers( size_t count )
            : mBuffers( count )
        {
        }

        ~CommunicationBuffers()
        {
            Clear();
        }

        void SetSize( size_t count )
        {
            if ( count < mCount )
            {
                for ( auto threadBuffers = mBuffers.begin() + mCount, end = mBuffers.end(); threadBuffers != end; ++threadBuffers )
                {
                    for ( auto &buffers : *threadBuffers )
                    {
                        for ( std::vector< char > *buffer : buffers.second )
                        {
                            delete buffer;
                        }
                    }
                }
            }

            mCount = count;
            mBuffers.resize( count );
        }

        std::vector< char > *Aquire( size_t size )
        {
            std::vector< std::vector< char > * > &buffers = mBuffers[gPID][size];

            if ( buffers.size() > 0 )
            {
                std::vector< char > *buffer = buffers.back();
                buffers.pop_back();
                return buffer;
            }
            else
            {
                return new std::vector< char >( size );
            }
        }

        void Release( std::vector< char > *buffer )
        {
            mBuffers[gPID][buffer->size()].push_back( buffer );
        }

        void Clear()
        {
            for ( auto &threadBuffers : mBuffers )
            {
                for ( auto &buffers : threadBuffers )
                {
                    for ( std::vector< char > *buffer : buffers.second )
                    {
                        delete buffer;
                    }
                }
            }
        }

    private:

        std::vector< std::unordered_map< size_t, std::vector< std::vector< char > * > >  > mBuffers;
        size_t mCount;
    };

    struct PutRequest
    {
        std::vector< char > *buffer;
        const void *destination;
        size_t globalId;
        ptrdiff_t offset;
    };

    struct GetRequest
    {
        const void *destination;
        const void *source;
        size_t size;
    };

    struct SendRequest
    {
        std::vector< char > buffer;
        std::vector< char > tag;
    };

    struct PushRequest
    {
        const void *pushRegister;
        RegisterInfo registerInfo;
    };

    struct PopRequest
    {
        const void *popRegister;
    };

    template< typename tQueue >
    class CommunicationQueues
    {
    public:

        CommunicationQueues( size_t nProcs )
            : mQueues( 0 ),
              mProcCount( nProcs )
        {
            ResetResize( nProcs );
        }

        CommunicationQueues()
            : mProcCount( 0 )
        {

        }

        void ResetResize( size_t maxProcs )
        {
            mQueues.clear();
            mQueues.resize( maxProcs );

            for ( auto &queue : mQueues )
            {
                queue.resize( maxProcs );
            }

            mProcCount = maxProcs;
        }

        tQueue &GetQueueToMe( size_t source )
        {
            return GetQueue( source, gPID );
        }

        tQueue &GetQueueFromMe( size_t target )
        {
            return GetQueue( gPID, target );
        }

    private:

        std::vector< std::vector< tQueue > > mQueues;

        size_t mProcCount;

        tQueue &GetQueue( size_t owner, size_t target )
        {
            return mQueues[owner][target];
        }
    };

    Barrier mThreadBarrier;
    CommunicationBuffers mThreadBuffers;

    CommunicationQueues< std::vector< PutRequest > > mPutRequests;
    CommunicationQueues< std::vector< GetRequest > > mGetRequests;
    CommunicationQueues< std::vector< SendRequest > > mTmpSendRequests;
    std::vector< std::vector< SendRequest > > mSendRequests;

    std::vector< size_t > mSendReceivedIndex;

    std::vector< std::vector< PushRequest > > mPushRequests;
    std::vector< std::vector< PopRequest > > mPopRequests;

    std::vector< size_t > mRegisterCount;
    std::vector<std::unordered_map< const void *, RegisterInfo > > mRegisters;
    std::vector< std::vector< const void * > > mThreadRegisterLocation;

    std::vector< std::future< void > > mThreads;
    std::function< void() > mEntry;
    std::vector< std::chrono::time_point< std::chrono::high_resolution_clock > > mStartTimes;
    size_t mProcCount;
    std::vector<size_t> mNewTagSize;
    size_t mTagSize;

    bool mEnded;
    std::atomic_bool mAbort;

    void StartTiming()
    {
        mStartTimes[gPID] = std::chrono::high_resolution_clock::now();
    }

    void SyncPoint()
    {
        mThreadBarrier.Wait( &mAbort );
    }

    void CheckAborted() const
    {
        if ( mAbort )
        {
            std::terminate();
        }
    }

};

inline void bsp_init( void( *spmd )( void ), int argc, char **argv )
{
    BSP::GetInstance().Init( spmd, argc, argv );
}

inline void bsp_begin( size_t P )
{
    BSP::GetInstance().Begin( P );
}

inline void bsp_end()
{
    BSP::GetInstance().End();
}

inline size_t bsp_pid()
{
    return BSP::GetInstance().PID();
}

inline size_t bsp_nprocs()
{
    return BSP::GetInstance().NProcs();
}

inline void bsp_abort( const char *error_message, ... )
{
    BSP::GetInstance().Abort( error_message );
}

inline void bsp_vabort( const char *error_message, va_list args )
{
    BSP::GetInstance().VAbort( error_message, args );
}

inline void bsp_sync()
{
    BSP::GetInstance().Sync();
}

inline double bsp_time()
{
    return BSP::GetInstance().Time();
}

inline void bsp_push_reg( const void *ident, size_t size )
{
    BSP::GetInstance().PushReg( ident, size );
}

inline void bsp_pop_reg( const void *ident )
{
    BSP::GetInstance().PopReg( ident );
}

inline void bsp_put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{
    BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
}

inline void bsp_get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{
    BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
}

inline void bsp_set_tagsize( size_t *size )
{
    BSP::GetInstance().SetTagsize( size );
}

inline void bsp_send( uint32_t pid, const void *tag, const void *payload, const size_t size )
{
    BSP::GetInstance().Send( pid, tag, payload, size );
}

inline void bsp_hpsend( uint32_t pid, const void *tag, const void *payload, const size_t size )
{
    BSP::GetInstance().Send( pid, tag, payload, size );
}

inline void bsp_qsize( size_t *packets, size_t *accumulated_size )
{
    BSP::GetInstance().QSize( packets, accumulated_size );
}

inline void bsp_get_tag( size_t *status, void *tag )
{
    BSP::GetInstance().GetTag( status, tag );
}

inline void bsp_move( void *payload, const size_t max_copy_size )
{
    BSP::GetInstance().Move( payload, max_copy_size );
}

inline size_t bsp_hpmove( void **tag_ptr, void **payload_ptr )
{
    BSP &bsp = BSP::GetInstance();
    size_t status;
    bsp.GetTag( &status, *tag_ptr );
    bsp.Move( *payload_ptr, status );
    return status;
}

inline void bsp_hpput( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{
    BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
}

inline void bsp_hpget( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{
    BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
}

#endif