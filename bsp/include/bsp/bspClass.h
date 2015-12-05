/**
 * Copyright (c) 2015 Mick van Duijn, Koen Visscher and Paul Visscher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#ifndef __BSPCLASS_H__
#define __BSPCLASS_H__

#include "bsp/communicationQueues.h"
#include "bsp/condVarBarrier.h"
#include "bsp/mixedBarrier.h"
#include "bsp/requests.h"
#include "bsp/barrier.h"

#include <assert.h>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <chrono>
#include <future>

// forward declaration of the main function
// so we can start this if no other function is given.
// E.G. Legacy behaviour of the MulticoreBSP library.
extern int main( int argc, char **argv );

/**
 * The BSP implementation class. By using this class as singleton, we avoid global memory declarations, cross source
 * problems with static variables and ensure proper usage from a header only library.
 */

class BSP
{
public:

    /**
     * Aborts the BSP program with the given error message. The formatting used is the same as in
     * fprintf.
     *
     * @param   format Describes the message to format.
     * @param   args   The formatting arguments.
     *
     * @post
     * * All threads other than main thread will exit on a BSP call, unless the BSP process is restarted.
     * * If main thread, a BSPAbort exception will be thrown.
     *
     * @see Abort( const char *format, ... )
     */

    BSP_FORCEINLINE void VAbort( const char *format, va_list args )
    {
        mAbort = true;
        vfprintf( stderr, format, args );
    }

    /**
     * Aborts the BSP program with the given error message. The formatting used is the same as in
     * fprintf.
     *
     * @param   format Describes the message to format.
     * @param   ...    Variable arguments providing message formatting.
     *
     * @post
     * * All threads other than main thread will exit on a BSP call, unless the BSP process is restarted.
     * * If main thread, a BSPAbort exception will be thrown.
     */

    inline void Abort( const char *format, ... )
    {
        va_list args;
        va_start( args, format );

        //pass to bsp_vabort
        VAbort( format, args );

        //mark end of variable arguments
        va_end( args );
    }

    /**
     * Gets the the amount of processors used by the BSP library. When the BSP library is not
     * initialised this returns the amount of processors available in hardware. After initalisation
     * it returns the maximum amount of processors available as initialised.
     *
     * @return The amount of processors available.
     *
     * @pre
     *  If Begin has been called:
     *      * Returns the number of started threads.
     *  Else:
     *      * Returns the number of physical threads available.
     */

    BSP_FORCEINLINE uint32_t NProcs() const
    {
        return mProcCount > 0 ? mProcCount : std::thread::hardware_concurrency();
    }

    /**
     * Get both the amount of messages, and the total size of the messages in bytes.
     *
     * @param [in,out]  packets         The packets count.
     * @param [in,out]  accumulatedSize If non-null, size of the accumulated packets in bytes.
     *
     * @pre
     *  * packets != nullptr.
     *  * Begin has been called.
     *
     * @post
     *  * If accumulatedSize != nullptr:
     *      accumulatedSize will be the accumulated size of the packets in bytes.
     *    Else:
     *      The calculation of the accumulated size will be skipped.
     */

    BSP_FORCEINLINE void QSize( size_t *packets, size_t *accumulatedSize )
    {
#ifndef BSP_SKIP_CHECKS
        assert( packets != nullptr );
#endif

        *packets = 0;

        if ( accumulatedSize )
        {
            *accumulatedSize = 0;
        }

        const std::vector< BspInternal::SendRequest > &sendQueue = mProcessorsData[ProcId()].sendRequests;
        *packets += sendQueue.size();

        if ( accumulatedSize )
        {
            for ( const auto &request : sendQueue )
            {
                *accumulatedSize += request.bufferSize;
            }
        }
    }

    /**
     * Gets the current processor id, which lies between 0 and NProcs() - 1.
     *
     * @return The current processor id.
     *
     * @pre Begin has been called, otherwise this will return 0xdeadbeef.
     */

    BSP_FORCEINLINE uint32_t &ProcId()
    {
        static thread_local uint32_t gPID = 0xdeadbeef;

        return gPID;
    }

    /**
     * Gets the time in seconds since for this thread since starting calculations by
     * calling Begin().
     *
     * @return The amount of time since calling Begin().
     *
     * @pre Begin has been called
     */

    BSP_FORCEINLINE double Time()
    {
        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mProcessorsData[ProcId()].startTime;
        return diff.count();
    }

    /**
     * Initialises the BSP computation process. Please note that the main thread should also call the entry function.
     *
     * @param   entry The entry function to execute.
     *
     * @pre
     *  * If the previous BSP program was terminated using Abort or VAbort:
     *      * BSPLib will try to end the previous BSP program by notifying all threads that are stuck in a
     *        synchronisation about the previous abort.
     *      * After 100 failed tries, the entire program will be terminated by std::terminate().
     *
     * @post
     *  * The previous BSP program has successfully been ended or aborted.
     *  * Tag size is 0.
     *  * Entry point is now entry.
     *  * For the main thread, ProcId() == 0.
     */

    BSP_FORCEINLINE void Init( std::function< void() > entry, int32_t, char ** )
    {
        mEntry = entry;
        mTagSize = 0;

        if ( !mEnded && !mAbort )
        {
            fprintf( stderr, "Warning: initialisation data corresponding to another BSP run found;\n" );
            fprintf( stderr, "         and this other run did not terminate (gracefully).\n" );
        }

        if ( mAbort )
        {
#ifndef BSP_SUPPRESS_ABORT_WARNING
            fprintf( stderr, "Warning: the previous BSP program ended using Abort or VAbort.\n" );
            fprintf( stderr, "         BSPLib will now try to determine whether all threads have stopped.\n\n" );

#   ifndef DEBUG
            uint32_t i = 0;
#   endif
#endif

            for ( auto &thr : mThreads )
            {
#ifndef BSP_SUPPRESS_ABORT_WARNING
#   ifndef DEBUG
                fprintf( stderr, "Determining the status of thread %d.\n", ++i );
#   endif
#endif
                uint32_t count = 0;

                while ( thr.wait_for( std::chrono::milliseconds( 200 ) ) == std::future_status::timeout && count++ < 100 )
                {
                    mThreadBarrier.NotifyAbort();
                }

                if ( count >= 100 )
                {
                    fprintf( stderr, "Error: could not safely end the previous BSP program. Terminating now." );
                    std::terminate();
                }
                else
                {
#ifndef BSP_SUPPRESS_ABORT_WARNING
#   ifndef DEBUG
                    fprintf( stderr, "        %d has ended.\n\n", i );
#   endif
#endif
                }
            }
        }

        ProcId() = 0;
    }

    /**
     * Begins the computations with the maximum given processors.
     *
     * @exception   e Thrown when an abort error condition occurs, if enabled with symbol BSP_THROW.
     *
     * @param   maxProcs The maximum processors to use in computation.
     *
     * @pre
     *  * If Init has been called:
     *      Will execute entry in parallel.
     *    Else:
     *      Will execute main in parallel. Also, a warning will be printed to stderr.
     *  * If called from the main thread:
     *      * Resets and initialises the communication queues and buffers.
     *      * Calls the entry point for all other threads.
     *      * Starts the timer for the main thread.
     *    Else:
     *      Starts the timer for the current thread.
     */

    void Begin( uint32_t maxProcs )
    {
        //fprintf( stderr, "Begin %ld-%ld\n", PID(), std::this_thread::get_id() );
        if ( !mEntry )
        {
            mEntry = [] { main( 0, nullptr ); };
            ProcId() = 0;
        }

        if ( ProcId() )
        {

#ifndef BSP_SKIP_CHECKS

            if ( ProcId() == 0xdeadbeef )
            {
                fprintf( stderr, "A processor with ID `0xdeadbeef` is found, this can happen when you forget to call Init()." );
            }

            assert( maxProcs == mProcCount );
            assert( ProcId() < maxProcs );
#endif

            StartTiming();
            return;
        }

        mAbort = false;
        mEnded = false;
        mProcCount = maxProcs;

        ResetChangedBooleans();

        mProcessorsData.clear();
        mProcessorsData.resize( maxProcs );

        mPutRequests.ResetResize( maxProcs );

        mGetRequests.ResetResize( maxProcs );

        mTmpSendRequests.ResetResize( maxProcs );
        mTmpSendBuffers.ResetResize( maxProcs );

        mThreadBarrier.SetSize( maxProcs );

        mThreads.clear();
        mThreads.reserve( maxProcs );

        for ( uint32_t i = 1; i < mProcCount; ++i )
        {
            mThreads.emplace_back( std::async( std::launch::async, [this]( uint32_t pid )
            {
                ProcId() = pid;

                try
                {
                    mEntry();
                }
                catch ( BspInternal::BspAbort & )
                {

                }
            }, i ) );
        }

        StartTiming();
    }

    /**
     * Ends the BSP computation.
     *
     * @pre Begin has been called
     *
     * @post
     *  * Ended is true
     *  * All threads have synced and ended.
     *  * The main thread releases all threads.
     */

    void End()
    {
        mEnded = true;

        SyncPoint();

        if ( ProcId() == 0 )
        {
            mThreads.clear();

            mProcCount = 0;
        }
    }

    /**
     * Synchronises all threads and communications.
     *
     * @pre Begin has been called
     *
     * @post
     *  * All threads have completed this superstep.
     *  * Tagsize has been synchronized and is available in the next superstep.
     *  * Get requests have been processed, updated data is available in the next superstep.
     *  * Pop requests have been processed, registers will be unavailable in the next superstep.
     *  * Put requests have been processed, updated data is available in the next superstep.
     *  * Buffers for the put requests have been cleared.
     *  * Push requests have been processed, registers will be available in the next superstep.
     */

    BSP_FORCEINLINE void Sync()
    {
        CheckAborted();

        uint32_t &pid = ProcId();

        SyncPoint();

        const bool registersChanged = mRegistersChanged;
        const bool tagSizeChanged = mTagSizeChanged;
        const bool hasGetRequests = mHasGetRequests;
        const bool hasPutRequests = hasGetRequests || mHasPutRequests;
        const bool hasSendRequests = mHasSendRequests;

        if ( tagSizeChanged && pid == 0 && mProcessorsData[0].newTagSize != mTagSize )
        {
            mTagSize = mProcessorsData[0].newTagSize;
        }

        if ( hasGetRequests )
        {
            ProcessGetRequests( pid );
        }

        if ( hasGetRequests || ( tagSizeChanged && hasSendRequests ) )
        {
            SyncPoint();
        }

        if ( registersChanged )
        {
            ProcessPopRequests( pid );
        }

        if ( hasSendRequests )
        {
            ProcessSendRequests( pid );
        }

        if ( hasPutRequests )
        {
            ProcessPutRequests( pid );
        }

        if ( hasPutRequests || hasSendRequests )
        {
            SyncPoint();
        }

        if ( hasPutRequests )
        {
            mProcessorsData[pid].putBufferStack.Clear();
        }

        if ( registersChanged )
        {
            ProcessPushRequests( pid );
        }

        SyncPoint();

        if ( !pid )
        {
            ResetChangedBooleans();
        }
    }

    /**
     * Pushes a register, with the given size.
     *
     * @param   ident The identifier.
     * @param   size  The size.
     *
     * @pre Begin has been called.
     *
     * @post
     *  * Push request has been queued.
     *  * In the next superstep, this register will be available for Put/Get.
     */

    BSP_FORCEINLINE void PushReg( const void *ident, size_t size )
    {
        mRegistersChanged = true;

        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mProcessorsData.size() > pid );
#endif

        mProcessorsData[pid].pushRequests.emplace_back( BspInternal::PushRequest{ ident, { size, mProcessorsData[pid].registerCount++ } } );
    }

    /**
     * Pops the register described by ident.
     *
     * @param   ident The identifier.
     *
     * @pre Begin has been called.
     *
     * @post
     *  * Pop request has been queued.
     *  * In the next superstep, this register will be unavailable for Put/Get.
     */

    void PopReg( const void *ident )
    {
        mRegistersChanged = true;

        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mProcessorsData.size() > pid );
#endif

        mProcessorsData[pid].popRequests.emplace_back( BspInternal::PopRequest{ ident } );
    }

    /**
     * Puts a buffer of size nbytes from source pointer src in the thread with ID pid at offset from destination pointer
     * dst.
     *
     * @param   pid         The processor ID.
     * @param   src         Source to read the buffer from.
     * @param [in,out]  dst Destination to write the buffer to.
     * @param   offset      The offset from the destination to start writing at.
     * @param   nbytes      The size of the message to be written to the other processor.
     *
     * @pre
     * * Begin has been called.
     * * src != nullptr.
     * * dst != nullptr.
     * * PushReg has been called on dst with at least size offset + nbytes in the processor with ID pid.
     * * A Sync has happened between PushReg and this call.
     */

    BSP_FORCEINLINE void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {
        mHasPutRequests = true;
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( tpid < mProcCount );
        assert( pid < mProcCount );
        assert( src && dst );
#endif

        const char *srcBuff = reinterpret_cast<const char *>( src );
        const size_t globalId = LocalToGlobal( tpid, dst ); //mRegisters[tpid][dst].registerCount;

#ifndef BSP_SKIP_CHECKS
        assert( mProcessorsData[pid].threadRegisterLocation.size() > globalId );

        /*mThreadRegisterLocation[pid][globalId]*/
        assert( mProcessorsData[pid].registers.find( GlobalToLocal( pid, globalId ) ) != mProcessorsData[pid].registers.end() );
        assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        const char *dstBuff = reinterpret_cast<const char *>( mProcessorsData[pid].threadRegisterLocation[globalId] );
        ptrdiff_t bufferLocation = mProcessorsData[tpid].putBufferStack.Alloc( nbytes, srcBuff );

        mPutRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::PutRequest{ bufferLocation, dstBuff + offset, nbytes } );
    }

    /**
     * Gets a buffer of size nbytes from source pointer src that is located in the thread with ID pid at offset from
     * source pointer src and stores it at the location of.
     *
     * @param   pid         The processor ID.
     * @param   src         Source to read the buffer from.
     * @param   offset      The offset from the source to start reading from.
     * @param [in,out]  dst Destination to write the buffer to.
     * @param   nbytes      The size of the message to be written in bytes.
     *
     * @pre
     * * Begin has been called.
     * * src != nullptr.
     * * dst != nullptr.
     * * PushReg has been called on dst with at leas size offset + nbytes in the processor with ID pid.
     * * A Sync has happened between PushReg and this call.
     */

    BSP_FORCEINLINE void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {
        mHasGetRequests = true;
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( tpid < mProcCount );
        assert( pid < mProcCount );
        assert( src && dst );
#endif

        const size_t globalId = LocalToGlobal( tpid, src ); //mRegisters[tpid][src].registerCount;

#ifndef BSP_SKIP_CHECKS
        assert( mProcessorsData[pid].threadRegisterLocation.size() > globalId );

        // mThreadRegisterLocation[pid][globalId]
        assert( mProcessorsData[pid].registers.find( GlobalToLocal( pid, globalId ) ) != mProcessorsData[pid].registers.end() );
        assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        const char *srcBuff = reinterpret_cast<const char *>( mProcessorsData[pid].threadRegisterLocation[globalId] );

        mGetRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::GetRequest{ dst, srcBuff + offset, nbytes } );
    }

    /**
     * Send a buffered message to the processor with ID pid using a tag to identify the message.
     *
     * @param   pid     The processor ID to send the message to.
     * @param   tag     The tag to identify the message with.
     * @param   payload The payload of the message.
     * @param   size    The size of the payload in bytes.
     *
     * @pre
     * * Begin has been called.
     * * Tagsize is equal on all threads.
     */

    BSP_FORCEINLINE void Send( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        mHasSendRequests = true;
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( tpid < mProcCount );
        assert( mProcessorsData[tpid].newTagSize == mTagSize );
#endif // !BSP_SKIP_CHECKS

        const char *srcBuff = reinterpret_cast<const char *>( payload );
        const char *tagBuff = reinterpret_cast<const char *>( tag );

        BspInternal::StackAllocator &tmpSendBuffer = mTmpSendBuffers.GetQueueFromMe( pid, tpid );

        BspInternal::StackAllocator::StackLocation bufferLocation = tmpSendBuffer.Alloc( size, srcBuff );
        BspInternal::StackAllocator::StackLocation tagLocation = tmpSendBuffer.Alloc( mTagSize, tagBuff );

        mTmpSendRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::SendRequest{ bufferLocation, size, tagLocation, mTagSize } );
    }

    /**
     * Moves the first message in the queue to the given payload destination.
     *
     * @param [in,out]  payload  The payload destination.
     * @param   max_copy_size_in The maximum size of the payload to copy.
     *
     * @pre
     * * Begin has been called.
     * * If the send queue is empty or the cursor is at/behind the end:
     *      Does nothing.
     *   Else:
     *      Moves the first message in the queue o the given payload destination.
     * * payload != nullptr.
     *
     * @post The queue cursor for the send queue is moved to the next message.
     */

    BSP_FORCEINLINE void Move( void *payload, size_t max_copy_size_in )
    {
        mHasSendRequests = true;
        uint32_t &pid = ProcId();
        ProcessorData &data = mProcessorsData[pid];

        if ( data.sendRequests.empty() || data.sendReceivedIndex >= data.sendRequests.size() )
        {
            return;
        }

        assert( payload );

        BspInternal::SendRequest &request = data.sendRequests[data.sendReceivedIndex++];

        const size_t copySize = std::min( max_copy_size_in, request.bufferSize );
        data.sendBuffers.Extract( request.bufferLocation, copySize, ( char * )payload );
    }

    /**
     * Sets a tagsize for the next superstep.
     *
     * @param [in,out]  size The new size for the tags.
     *
     * @pre
     * * Begin has been called.
     * * size != nullptr.
     */

    BSP_FORCEINLINE void SetTagsize( size_t *size )
    {
        mTagSizeChanged = true;
        assert( size );
        const size_t newSize = *size;
        *size = mTagSize;
        mProcessorsData[ProcId()].newTagSize = newSize;
    }

    /**
     * Gets the tag for the next message in the send queue.
     *
     * @param [in,out]  status The output destination for the status of the queue.
     * @param [in,out]  tag    The output destination for the tag of the first message in the queue.
     *
     * @pre
     * * Begin has been called.
     * * If the send queue is empty or the cursor is at/behind the end of the queue:
     *      Returns a status equal to -1.
     *   Else:
     *      * Returns the size of the buffer of the request as status.
     *      * Writes the tag to the desired destination.
     *
     * @post The queue is in the same state as before.
     */

    BSP_FORCEINLINE void GetTag( size_t *status, void *tag )
    {
        uint32_t &pid = ProcId();
        *status = ( size_t ) - 1;
        ProcessorData &data = mProcessorsData[pid];

        size_t index = data.sendReceivedIndex;
        const std::vector< BspInternal::SendRequest > &sendQueue = data.sendRequests;

        if ( !sendQueue.empty() && index < sendQueue.size() )
        {
            const BspInternal::SendRequest &sendRequest = sendQueue[index];
            *status = sendRequest.bufferSize;

            char *tagBuff = reinterpret_cast<char *>( tag );

#ifndef BSP_SKIP_CHECKS
            assert( sendQueue[index].tagSize == mTagSize );
#endif // !BSP_SKIP_CHECKS

            data.sendBuffers.Extract( sendRequest.tagLocation, sendRequest.tagSize, tagBuff );
        }
    }

    /**
     * Query if this object is ended.
     *
     * @return true if ended, false if not.
     */

    BSP_FORCEINLINE bool IsEnded() const
    {
        return mEnded;
    }

    /**
     * Gets the static instance of the BSP class to support static calls.
     *
     * @return The instance.
     */

    static BSP_FORCEINLINE BSP &GetInstance()
    {
        static BSP mBSP;
        return mBSP;
    }

private:

    struct ProcessorData
    {
        ProcessorData()
            : sendReceivedIndex( 0 ),
              registerCount( 0 ),
              newTagSize( 0 ),
              sendRequestsSize( 0 ),
              pushRequestsSize( 0 ),
              popRequestsSize( 0 ),
              putBufferStack( 9064 ),
              sendBuffers( 9064 )
        {
            sendRequests.reserve( 9064 );
            pushRequests.reserve( 9064 );
            popRequests.reserve( 9064 );
        }

        size_t sendReceivedIndex;
        size_t registerCount;
        size_t newTagSize;
        size_t sendRequestsSize;
        size_t pushRequestsSize;
        size_t popRequestsSize;
        BspInternal::StackAllocator putBufferStack;
        BspInternal::StackAllocator sendBuffers;
        std::chrono::time_point< std::chrono::high_resolution_clock > startTime;
        std::vector< BspInternal::SendRequest > sendRequests;
        std::vector< BspInternal::PushRequest > pushRequests;
        std::vector< BspInternal::PopRequest > popRequests;
        std::map< const void *, BspInternal::RegisterInfo > registers;
        std::vector< const void * > threadRegisterLocation;
    };

    BspInternal::MixedBarrier mThreadBarrier;

    BspInternal::CommunicationQueues< std::vector< BspInternal::PutRequest > > mPutRequests;
    BspInternal::CommunicationQueues< std::vector< BspInternal::GetRequest > > mGetRequests;

    BspInternal::CommunicationQueues< std::vector< BspInternal::SendRequest > > mTmpSendRequests;
    BspInternal::CommunicationQueues< BspInternal::StackAllocator > mTmpSendBuffers;

    std::vector< ProcessorData > mProcessorsData;

    std::vector< std::future< void > > mThreads;
    std::function< void() > mEntry;
    uint32_t mProcCount;
    std::atomic_size_t mTagSize;

    volatile bool mRegistersChanged;
    volatile bool mTagSizeChanged;
    volatile bool mHasGetRequests;
    volatile bool mHasPutRequests;
    volatile bool mHasSendRequests;

    bool mEnded;
    std::atomic_bool mAbort;

    BSP()
        : mThreadBarrier( 0 ),
          mProcCount( 0 ),
          mTagSize( 0 ),
          mEnded( true ),
          mAbort( false )
    {
    }

    void ResetChangedBooleans()
    {
        mRegistersChanged = false;
        mTagSizeChanged = false;
        mHasGetRequests = false;
        mHasPutRequests = false;
        mHasSendRequests = false;
    }

    void StartTiming()
    {
        assert( ProcId() != 0xdeadbeef );
        mProcessorsData[ProcId()].startTime = std::chrono::high_resolution_clock::now();
    }

    void SyncPoint()
    {
        mThreadBarrier.Wait( mAbort );
    }

    void CheckAborted()
    {
        if ( mAbort )
        {
            mThreadBarrier.NotifyAbort();
            throw BspInternal::BspAbort( "Aborted" );
        }
    }

    BSP_FORCEINLINE void ProcessPushRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        if ( !data.pushRequests.empty() )
        {
            for ( const auto &pushRequest : data.pushRequests )
            {
                data.registers[pushRequest.pushRegister] = pushRequest.registerInfo;
                data.threadRegisterLocation.push_back( pushRequest.pushRegister );
            }

            data.pushRequests.clear();
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
                    mProcessorsData[owner].putBufferStack.Extract( putRequest->bufferLocation, putRequest->size,
                                                                   ( char * )putRequest->destination );
                }

                putQueue.clear();
            }
        }
    }

    BSP_FORCEINLINE void ProcessSendRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];
        data.sendRequests.clear();
        data.sendReceivedIndex = 0;

        BspInternal::StackAllocator::StackLocation offset = 0;
        BspInternal::StackAllocator &sendBuffer = data.sendBuffers;

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
                           std::back_insert_iterator< std::vector< BspInternal::SendRequest > >( data.sendRequests ) );
                tmpQueue = std::vector< BspInternal::SendRequest >();

                BspInternal::StackAllocator &tmpBuffer = mTmpSendBuffers.GetQueueToMe( owner, pid );

                offset += tmpBuffer.Size();
                sendBuffer.Merge( tmpBuffer );
                tmpBuffer.Clear();
            }
        }
    }

    BSP_FORCEINLINE void ProcessPopRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        if ( !data.popRequests.empty() )
        {
            for ( const auto &popRequest : data.popRequests )
            {
                data.registers.erase( popRequest.popRegister );
            }

            data.popRequests.clear();
        }
    }

    BSP_FORCEINLINE void ProcessGetRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::GetRequest > &getQueue = mGetRequests.GetQueueToMe( owner, pid );

            for ( auto request = getQueue.rbegin(), end = getQueue.rend(); request != end; ++request )
            {
                const char *srcBuff = reinterpret_cast<const char *>( request->source );

                BspInternal::StackAllocator::StackLocation bufferLocation = data.putBufferStack.Alloc( request->size, srcBuff );

                mPutRequests.GetQueueFromMe( owner, pid ).emplace_back( BspInternal::PutRequest{ bufferLocation, request->destination, request->size } );
            }

            getQueue.clear();
        }
    }

    BSP_FORCEINLINE size_t LocalToGlobal( uint32_t pid, const void *reg )
    {
#ifndef BSP_SKIP_CHECKS
        assert( mProcessorsData[pid].registers.find( reg ) != mProcessorsData[pid].registers.end() );
#endif
        return ( *mProcessorsData[pid].registers.find( reg ) ).second.registerCount;
    }

    BSP_FORCEINLINE const void *GlobalToLocal( uint32_t pid, size_t globalId )
    {
        return mProcessorsData[pid].threadRegisterLocation[globalId];
    }

};

#endif