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
#ifndef __BSPLIB_BSPCLASS_H__
#define __BSPLIB_BSPCLASS_H__

#define BSP_SKIP_CHECKS

#include "bsp/threadRegisterDenseHash.h"
#include "bsp/threadRegisterVector.h"
#include "bsp/communicationQueues.h"
#include "bsp/threadRegisterMap.h"
#include "bsp/condVarBarrier.h"
#include "bsp/mixedBarrier.h"
#include "bsp/barrier.h"

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <stdarg.h>
#include <atomic>
#include <chrono>
#include <future>
#include <map>

#ifdef _WIN32
#include <windows.h>
#undef max
#undef min
#endif // _WIN32

// forward declaration of the main function
// so we can start this if no other function is given.
// E.G. Legacy behaviour of the MulticoreBSP library.
extern int main( int argc, char **argv );

#ifndef BSP_BARRIER_TYPE
typedef BspInternal::MixedBarrier tBarrier;
#else
typedef BSP_BARRIER_TYPE tBarrier;
#endif

#ifndef BSP_REGISTERMAP_TYPE
typedef BspInternal::ThreadRegisterVector tRegisterMap;
#else
typedef BSP_REGISTERMAP_TYPE tRegisterMap;
#endif

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

    inline void VAbort( const char *format, va_list args )
    {
        mAbort = true;
        vfprintf( stderr, format, args );
        CheckAborted();
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

    inline uint32_t NProcs() const
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

    inline void QSize( size_t *packets, size_t *accumulatedSize )
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

    inline uint32_t &ProcId()
    {
        //static thread_local uint32_t gPID = 0xdeadbeef;
        static BSP_TLS uint32_t gPID = 0xdeadbeef;

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

    inline double Time()
    {
        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mProcessorsData[ProcId()].startTime;
        return diff.count();
    }

    inline void Tic()
    {
        mProcessorsData[ProcId()].ticTime = std::chrono::high_resolution_clock::now();
    }

    inline double Toc()
    {
        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mProcessorsData[ProcId()].ticTime;
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

    inline void Init( std::function< void() > entry, int32_t, char ** )
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

    inline void Begin( uint32_t maxProcs )
    {
        //fprintf( stderr, "Begin %ld-%ld\n", PID(), std::this_thread::get_id() );
        if ( !mEntry )
        {
            mEntry = [] { main( 0, nullptr ); };
            ProcId() = 0;
        }

#ifdef _WIN32
        SetThreadAffinityMask( GetCurrentThread(), 1 << ( ProcId() % std::thread::hardware_concurrency() ) );
        SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
#endif // _WIN32

#ifdef __GNUG__
        /*sched_param sch;
        int policy;
        pthread_getschedparam(pthread_self(), &policy, &sch);
        sch.sched_priority = sched_get_priority_max(SCHED_FIFO);
        int shedErr = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch);
        assert(shedErr == 0);
        pthread_getschedparam(pthread_self(), &policy, &sch);*/
        int num_cores = std::thread::hardware_concurrency();
        int core_id = ProcId() % num_cores;

        cpu_set_t cpuset;
        CPU_ZERO( &cpuset );
        CPU_SET( core_id, &cpuset );

        pthread_t current_thread = pthread_self();
        pthread_setaffinity_np( current_thread, sizeof( cpu_set_t ), &cpuset );
#endif


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

        mProcessorsData.clear();
        mProcessorsData.resize( maxProcs );

        mPutRequests.ResetResize( maxProcs );

        mGetRequests.ResetResize( maxProcs );
        mBufferedGetRequests.ResetResize( maxProcs );

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

    inline void End()
    {
        mEnded = true;

        SyncPoint();

        if ( ProcId() == 0 )
        {
            mThreads.clear();

            mProcCount = 0;
        }
    }

    inline void SyncPoint()
    {
        mThreadBarrier.Wait( mAbort );
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

    inline void Sync()
    {
        CheckAborted();

        uint32_t &pid = ProcId();

        ProcessorData &data = mProcessorsData[pid];
        auto &syncBools = data.syncBools;

        CheckSyncBools( pid );

        SyncPoint();

        bool anySync = CollectSyncBools( pid );

        if ( syncBools.hasTagSizeUpdate )
        {
            //printf( "%d updates tagsize\n", pid );
            ProcessTagSizeUpdate( pid );
        }

        if ( syncBools.hasGetRequests )
        {
            //printf( "%d buffers get\n", pid );
            BufferGetRequests( pid );
        }

        if ( syncBools.hasTagSizeUpdate || syncBools.hasGetRequests )
        {
            //printf( "%d syncs tagsize or get\n", pid );
            SyncPoint();
        }

        if ( syncBools.hasPopRequests )
        {
            //printf( "%d processes pop\n", pid );
            ProcessPopRequests( pid );
        }

        if ( syncBools.hasSendRequests )
        {
            //printf( "%d processes send\n", pid );
            ProcessSendRequests( pid );
        }

        if ( syncBools.hasPutRequests )
        {
            //printf( "%d processes put\n", pid );
            ProcessPutRequests( pid );
        }

        if ( syncBools.hasGetRequests )
        {
            //printf( "%d processes get\n", pid );
            ProcessGetRequests( pid );
        }

        if ( syncBools.hasSendRequests || syncBools.hasPopRequests || syncBools.hasPutRequests || syncBools.hasGetRequests )
        {
            //printf( "%d enters massive sync\n", pid );
            SyncPoint();
        }

        if ( syncBools.hasSendRequests )
        {
            //printf( "%d clears send\n", pid );
            ClearSendRequests( pid );
        }

        for ( uint32_t target = 0; target < mProcCount; ++target )
        {
            assert( mTmpSendBuffers.GetQueueFromMe( target, pid ).Empty() );
            assert( mTmpSendRequests.GetQueueFromMe( target, pid ).empty() );
        }

        if ( syncBools.hasPutRequests )
        {
            //printf( "%d clears put\n", pid );
            data.putBufferStack.Clear();
        }

        if ( syncBools.hasPushRequests )
        {
            //printf( "%d processes push\n", pid );
            ProcessPushRequests( pid );
            SyncPoint();
        }

        ResetBools( pid );

        if ( !anySync )
        {
            SyncPoint();
        }
    }

    inline void SyncPutRequests()
    {
        uint32_t &pid = ProcId();

        SyncPoint();

        ProcessPutRequests( pid );

        SyncPoint();

        mProcessorsData[pid].putBufferStack.Clear();
    }

    inline void SyncGetRequests()
    {
        uint32_t &pid = ProcId();

        SyncPoint();

        BufferGetRequests( pid );

        SyncPoint();

        ProcessGetRequests( pid );
    }

    inline void SyncSendRequests()
    {
        uint32_t &pid = ProcId();

        SyncPoint();

        ProcessSendRequests( pid );

        SyncPoint();

        ClearSendRequests( pid );
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

    void PushReg( const void *ident, size_t size )
    {
        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mProcessorsData.size() > pid );
#endif

        mProcessorsData[pid].pushRequests.emplace_back( BspInternal::PushRequest { ident, { ( uint32_t )size, mProcessorsData[pid].registerCount++ } } );
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
        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( mProcessorsData.size() > pid );
#endif

        mProcessorsData[pid].popRequests.emplace_back( BspInternal::PopRequest { ident } );
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
     * * Push has been called on dst with at least size offset + nbytes in the processor with ID pid.
     * * A Sync has happened between PushReg and this call.
     */

    BSP_FORCEINLINE void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {
        uint32_t &tpid = ProcId();
        //mHasPutRequests[mProcessorsData[tpid].syncBoolIndex] = true;

#ifndef BSP_SKIP_CHECKS
        assert( tpid < mProcCount );
        assert( pid < mProcCount );
        assert( src && dst );
#endif

        const char *srcBuff = reinterpret_cast<const char *>( src );
        const uint32_t globalId = mProcessorsData[tpid].threadRegisters.LocalToGlobal( dst );

#ifndef BSP_SKIP_CHECKS
        assert( mProcessorsData[pid].threadRegisterLocation.size() > globalId );

        /*mThreadRegisterLocation[pid][globalId]*/
        //assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        //const char *dstBuff = reinterpret_cast<const char *>( GlobalToLocal( pid, globalId ) );
        ptrdiff_t bufferLocation = mProcessorsData[tpid].putBufferStack.Alloc( nbytes, srcBuff );

        mPutRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::PutRequest { bufferLocation, offset, globalId, ( uint32_t )nbytes } );
    }

    /**
     * Gets a buffer of size nbytes from source pointer src that is located in the thread with ID pid at offset from
     * source pointer src and stores it at the location of dst.
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
     * * Push has been called on src with at leas size offset + nbytes in the processor with ID pid.
     * * A Sync has happened between PushReg and this call.
     */

    inline void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( tpid < mProcCount );
        assert( pid < mProcCount );
        assert( src && dst );
#endif

        const uint32_t globalId = mProcessorsData[tpid].threadRegisters.LocalToGlobal(
                                      src ); //mRegisters[tpid][src].registerCount;

#ifndef BSP_SKIP_CHECKS
        assert( mProcessorsData[pid].threadRegisterLocation.size() > globalId );
        //assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        //const char *srcBuff = reinterpret_cast<const char *>( GlobalToLocal( pid, globalId ) );

        mGetRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::GetRequest { dst, globalId, offset, ( uint32_t )nbytes } );
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

    inline void Send( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert( pid < mProcCount );
        assert( tpid < mProcCount );
#endif // !BSP_SKIP_CHECKS
        assert( mProcessorsData[tpid].newTagSize == mTagSize );

        const char *srcBuff = reinterpret_cast<const char *>( payload );
        const char *tagBuff = reinterpret_cast<const char *>( tag );

        BspInternal::StackAllocator &tmpSendBuffer = mTmpSendBuffers.GetQueueFromMe( pid, tpid );

        BspInternal::StackAllocator::StackLocation bufferLocation = tmpSendBuffer.Alloc( size, srcBuff );
        BspInternal::StackAllocator::StackLocation tagLocation = tmpSendBuffer.Alloc( mTagSize, tagBuff );

        mTmpSendRequests.GetQueueFromMe( pid, tpid ).emplace_back( BspInternal::SendRequest { bufferLocation, ( uint32_t )size, tagLocation, mTagSize } );
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

    inline void Move( void *payload, size_t max_copy_size_in )
    {
        uint32_t &pid = ProcId();
        ProcessorData &data = mProcessorsData[pid];

        if ( data.sendRequests.empty() || data.sendReceivedIndex >= data.sendRequests.size() )
        {
            return;
        }

        assert( payload );

        BspInternal::SendRequest &request = data.sendRequests[data.sendReceivedIndex++];

        const size_t copySize = std::min( ( uint32_t )max_copy_size_in, request.bufferSize );
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

    inline void SetTagsize( size_t *size )
    {
        uint32_t &pid = ProcId();
        assert( size );
        const uint32_t newSize = ( uint32_t ) * size;
        *size = mTagSize;
        mProcessorsData[pid].newTagSize = newSize;
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

    inline void GetTag( size_t *status, void *tag )
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

    inline bool IsEnded() const
    {
        return mEnded;
    }

    /**
     * Gets the static instance of the BSP class to support static calls.
     *
     * @return The instance.
     */

    static inline BSP &GetInstance()
    {
        static BSP mBSP;
        return mBSP;
    }

    BSP()
        : mThreadBarrier( 0 ),
          mProcCount( 0 ),
          mTagSize( 0 ),
          mEnded( true ),
          mAbort( false )
    {
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
              getBufferStack( 9064 ),
              sendBuffers( 9064 )
        {
            sendRequests.reserve( 9064 );
            pushRequests.reserve( 9064 );
            popRequests.reserve( 9064 );
        }

        uint32_t sendReceivedIndex;
        uint32_t registerCount;
        uint32_t newTagSize;
        uint32_t sendRequestsSize;
        uint32_t pushRequestsSize;
        uint32_t popRequestsSize;
        BspInternal::StackAllocator putBufferStack;
        BspInternal::StackAllocator getBufferStack;
        BspInternal::StackAllocator sendBuffers;
        std::chrono::time_point< std::chrono::high_resolution_clock > startTime;
        std::chrono::time_point< std::chrono::high_resolution_clock > ticTime;
        std::vector< BspInternal::SendRequest > sendRequests;
        std::vector< BspInternal::PushRequest > pushRequests;
        std::vector< BspInternal::PopRequest > popRequests;
        tRegisterMap threadRegisters;

        struct
        {
            bool hasPutRequests = false;
            bool hasGetRequests = false;
            bool hasPushRequests = false;
            bool hasPopRequests = false;
            bool hasSendRequests = false;
            bool hasTagSizeUpdate = false;
        } syncBools;
    };

    tBarrier mThreadBarrier;

    BspInternal::CommunicationQueues< std::vector< BspInternal::PutRequest > > mPutRequests;
    BspInternal::CommunicationQueues< std::vector< BspInternal::GetRequest > > mGetRequests;
    BspInternal::CommunicationQueues< std::vector< BspInternal::BufferedGetRequest > > mBufferedGetRequests;

    BspInternal::CommunicationQueues< std::vector< BspInternal::SendRequest > > mTmpSendRequests;
    BspInternal::CommunicationQueues< BspInternal::StackAllocator > mTmpSendBuffers;

    std::vector< ProcessorData > mProcessorsData;

    std::vector< std::future< void > > mThreads;
    std::function< void() > mEntry;
    uint32_t mProcCount;
    std::atomic< uint32_t > mTagSize;

    bool mEnded;
    std::atomic_bool mAbort;

    inline void StartTiming()
    {
        assert( ProcId() != 0xdeadbeef );
        mProcessorsData[ProcId()].startTime = std::chrono::high_resolution_clock::now();
        SyncPoint();
    }

    inline void CheckAborted()
    {
        if ( mAbort )
        {
            mThreadBarrier.NotifyAbort();
            throw BspInternal::BspAbort( "Aborted" );
        }
    }

    inline void CheckSyncBools( uint32_t pid )
    {
        CheckHasGetRequests( pid );
        CheckHasPopRequests( pid );
        CheckHasPushRequests( pid );
        CheckHasPutRequests( pid );
        CheckHasSendRequests( pid );
        CheckHasTagSizeUpdate( pid );
    }

    inline bool CollectSyncBools( uint32_t pid )
    {
        auto &syncBools = mProcessorsData[pid].syncBools;

        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            if ( !syncBools.hasSendRequests && mProcessorsData[owner].syncBools.hasSendRequests )
            {
                printf( "%d has no send, but %d does\n", pid, owner );
            }

            syncBools.hasGetRequests |= mProcessorsData[owner].syncBools.hasGetRequests;
            syncBools.hasPopRequests |= mProcessorsData[owner].syncBools.hasPopRequests;
            syncBools.hasPushRequests |= mProcessorsData[owner].syncBools.hasPushRequests;
            syncBools.hasPutRequests |= mProcessorsData[owner].syncBools.hasPutRequests;
            syncBools.hasSendRequests |= mProcessorsData[owner].syncBools.hasSendRequests;
            syncBools.hasTagSizeUpdate |= mProcessorsData[owner].syncBools.hasTagSizeUpdate;
        }

        return syncBools.hasGetRequests || syncBools.hasPopRequests || syncBools.hasPushRequests || syncBools.hasPutRequests ||
               syncBools.hasSendRequests || syncBools.hasTagSizeUpdate;
    }

    inline void ResetBools( uint32_t pid )
    {
        auto &syncBools = mProcessorsData[pid].syncBools;
        syncBools.hasGetRequests = false;
        syncBools.hasPopRequests = false;
        syncBools.hasPushRequests = false;
        syncBools.hasPutRequests = false;
        syncBools.hasSendRequests = false;
        syncBools.hasTagSizeUpdate = false;
    }

    inline void CheckHasPushRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasPushRequests = !data.pushRequests.empty();
    }

    inline void ProcessPushRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        if ( !data.pushRequests.empty() )
        {
            for ( const auto &pushRequest : data.pushRequests )
            {
                data.threadRegisters.Insert( pushRequest.pushRegister, pushRequest.registerInfo );
            }

            data.pushRequests.clear();
        }
    }

    inline void CheckHasPutRequests( uint32_t pid )
    {
        volatile bool &hasPutRequests = mProcessorsData[pid].syncBools.hasPutRequests;
        hasPutRequests = false;

        for ( size_t target = 0; !hasPutRequests && target < mProcCount; ++target )
        {
            hasPutRequests = !mPutRequests.GetQueueFromMe( target, pid ).empty();
        }
    }

    inline void ProcessPutRequests( uint32_t pid )
    {
        for ( size_t owner = pid; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::PutRequest > &putQueue = mPutRequests.GetQueueToMe( owner, pid );

            if ( !putQueue.empty() )
            {
                for ( auto putRequest = putQueue.rbegin(), end = putQueue.rend(); putRequest != end; ++putRequest )
                {
                    char *dstBuff = static_cast<char *>( const_cast<void *>( mProcessorsData[pid].threadRegisters.GlobalToLocal(
                                                                                 putRequest->globalId ) ) )
                                    + putRequest->offset;

                    mProcessorsData[owner].putBufferStack.Extract( putRequest->bufferLocation, putRequest->size, dstBuff );
                }

                putQueue.clear();
            }
        }

        for ( size_t owner = 0; owner < pid; ++owner )
        {
            std::vector< BspInternal::PutRequest > &putQueue = mPutRequests.GetQueueToMe( owner, pid );

            if ( !putQueue.empty() )
            {
                for ( auto putRequest = putQueue.rbegin(), end = putQueue.rend(); putRequest != end; ++putRequest )
                {
                    char *dstBuff = static_cast<char *>( const_cast<void *>( mProcessorsData[pid].threadRegisters.GlobalToLocal(
                                                                                 putRequest->globalId ) ) )
                                    + putRequest->offset;

                    mProcessorsData[owner].putBufferStack.Extract( putRequest->bufferLocation, putRequest->size, dstBuff );
                }

                putQueue.clear();
            }
        }
    }

    inline void CheckHasGetRequests( uint32_t pid )
    {
        volatile bool &hasGetRequests = mProcessorsData[pid].syncBools.hasGetRequests;
        hasGetRequests = false;

        for ( size_t target = 0; !hasGetRequests && target < mProcCount; ++target )
        {
            hasGetRequests = !mGetRequests.GetQueueFromMe( target, pid ).empty();
        }
    }

    inline void BufferGetRequests( uint32_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];
        data.getBufferStack.Clear();

        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::GetRequest > &getQueue = mGetRequests.GetQueueToMe( owner, pid );

            for ( auto request = getQueue.rbegin(), end = getQueue.rend(); request != end; ++request )
            {
                //const char *srcBuff = reinterpret_cast<const char *>( request->source );
                const char *srcBuff = reinterpret_cast<const char *>( mProcessorsData[pid].threadRegisters.GlobalToLocal(
                                                                          request->globalId ) ) + request->offset;

                BspInternal::StackAllocator::StackLocation bufferLocation = data.getBufferStack.Alloc( request->size, srcBuff );

                mBufferedGetRequests.GetQueueFromMe( owner, pid ).emplace_back( BspInternal::BufferedGetRequest { bufferLocation, request->destination, request->size } );
            }

            getQueue.clear();
        }
    }

    inline void ProcessGetRequests( uint32_t pid )
    {
        for ( size_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::BufferedGetRequest > &getQueue = mBufferedGetRequests.GetQueueToMe( owner, pid );

            if ( !getQueue.empty() )
            {
                for ( auto getRequest = getQueue.rbegin(), end = getQueue.rend(); getRequest != end; ++getRequest )
                {
                    char *dstBuff = static_cast< char * >( const_cast< void * >( getRequest->destination ) );

                    mProcessorsData[owner].getBufferStack.Extract( getRequest->bufferLocation, getRequest->size, dstBuff );
                }

                getQueue.clear();
            }
        }
    }

    inline void CheckHasSendRequests( uint32_t pid )
    {
        volatile bool &hasSendRequests = mProcessorsData[pid].syncBools.hasSendRequests;
        hasSendRequests = false;

        for ( size_t target = 0; !hasSendRequests && target < mProcCount; ++target )
        {
            hasSendRequests = !mTmpSendBuffers.GetQueueFromMe( target, pid ).Empty();
        }
    }



    inline void ProcessSendRequests( uint32_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        BspInternal::StackAllocator::StackLocation offset = 0;
        BspInternal::StackAllocator &sendBuffer = data.sendBuffers;
        std::vector< BspInternal::SendRequest > &sendQueue = data.sendRequests;
        sendQueue.clear();
        sendBuffer.Clear();
        data.sendReceivedIndex = 0;

        for ( uint32_t owner = 0; owner < mProcCount; ++owner )
        {
            std::vector< BspInternal::SendRequest > &tmpQueue = mTmpSendRequests.GetQueueToMe( owner, pid );
            BspInternal::StackAllocator &tmpBuffer = mTmpSendBuffers.GetQueueToMe( owner, pid );

            for ( auto request = tmpQueue.rbegin(), end = tmpQueue.rend(); request != end; ++request )
            {
                request->bufferLocation += offset;
                request->tagLocation += offset;
            }

            offset += tmpBuffer.Size();
            sendBuffer.Merge( tmpBuffer );
            sendQueue.insert( sendQueue.end(), tmpQueue.begin(), tmpQueue.end() );
        }
    }

    inline void ClearSendRequests( uint32_t pid )
    {
        for ( uint32_t target = 0; target < mProcCount; ++target )
        {
            mTmpSendBuffers.GetQueueFromMe( target, pid ).Clear();
            mTmpSendRequests.GetQueueFromMe( target, pid ).clear();
        }
    }

    inline void CheckHasPopRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasPopRequests = !data.popRequests.empty();
    }

    inline void ProcessPopRequests( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];

        if ( !data.popRequests.empty() )
        {
            for ( const auto &popRequest : data.popRequests )
            {
                data.threadRegisters.Erase( popRequest.popRegister );
            }

            data.popRequests.clear();
        }
    }

    inline void CheckHasTagSizeUpdate( size_t pid )
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasTagSizeUpdate = data.newTagSize != mTagSize;
    }

    inline void ProcessTagSizeUpdate( size_t pid )
    {
        if ( pid == 0 )
        {
            mTagSize = mProcessorsData[0].newTagSize;
        }
    }
};

#endif