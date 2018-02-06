/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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
 *
 * @endcond
 */
#pragma once
#ifndef __BSPLIB_BSPCLASS_H__
#define __BSPLIB_BSPCLASS_H__

//#define BSP_SKIP_CHECKS

#include "bsp/communicationQueues.h"
#include "bsp/historyRecorderType.h"
#include "bsp/condVarBarrier.h"
#include "bsp/processorData.h"
#include "bsp/requestVector.h"
#include "bsp/mixedBarrier.h"
#include "bsp/barrier.h"
#include "bsp/util.h"

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <stdarg.h>
#include <valarray>
#include <atomic>
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
extern int main(int argc, char **argv);

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

    inline void VAbort(const char *format, va_list args)
    {
        mAbort = true;
        vfprintf(stderr, format, args);
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

    inline void Abort(const char *format, ...)
    {
        va_list args;
        va_start(args, format);

        //pass to bsp_vabort
        VAbort(format, args);

        //mark end of variable arguments
        va_end(args);
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

    inline void QSize(size_t *packets, size_t *accumulatedSize)
    {
#ifndef BSP_SKIP_CHECKS
        assert(packets != nullptr);
#endif

        *packets = 0;

        if (accumulatedSize)
        {
            *accumulatedSize = 0;
        }

        const BSPInternal::RequestVector< BSPInternal::SendRequest > &sendQueue = mProcessorsData[ProcId()].sendRequests;
        *packets += sendQueue.GetSize();

        if (accumulatedSize)
        {
            for (auto request = sendQueue.CBegin(), end = sendQueue.CEnd(); request != end; ++request)
            {
                *accumulatedSize += request->bufferSize;
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
        return mProcessorsData[ProcId()].startTimer.Toc();
    }

    BSPUtil::TicTimer Tic()
    {
        mProcessorsData[ProcId()].ticTimer.Tic();
        return mProcessorsData[ProcId()].ticTimer;
    }

    inline double Toc()
    {
        return mProcessorsData[ProcId()].ticTimer.Toc();
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

    inline void Init(std::function< void() > entry, int32_t, char **)
    {
        mEntry = entry;
        mTagSize = 0;

        if (!mEnded && !mAbort)
        {
            fprintf(stderr, "Warning: initialisation data corresponding to another BSP run found;\n");
            fprintf(stderr, "         and this other run did not terminate (gracefully).\n");
        }

        if (mAbort)
        {
#ifndef BSP_SUPPRESS_ABORT_WARNING
            fprintf(stderr, "Warning: the previous BSP program ended using Abort or VAbort.\n");
            fprintf(stderr, "         BSPLib will now try to determine whether all threads have stopped.\n\n");

#   ifndef DEBUG
            uint32_t i = 0;
#   endif
#endif

            for (auto &thr : mThreads)
            {
#ifndef BSP_SUPPRESS_ABORT_WARNING
#   ifndef DEBUG
                fprintf(stderr, "Determining the status of thread %d.\n", ++i);
#   endif
#endif
                uint32_t count = 0;

                while (thr.wait_for(std::chrono::milliseconds(200)) == std::future_status::timeout && count++ < 100)
                {
                    mThreadBarrier.NotifyAbort();
                }

                if (count >= 100)
                {
                    fprintf(stderr, "Error: could not safely end the previous BSP program. Terminating now.");
                    std::terminate();
                }
                else
                {
#ifndef BSP_SUPPRESS_ABORT_WARNING
#   ifndef DEBUG
                    fprintf(stderr, "        %d has ended.\n\n", i);
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

    inline void Begin(uint32_t maxProcs)
    {
        //fprintf( stderr, "Begin %ld-%ld\n", PID(), std::this_thread::get_id() );
        if (!mEntry)
        {
            mEntry = [] { main(0, nullptr); };
            ProcId() = 0;
        }

#ifdef _WIN32
        SetThreadAffinityMask(GetCurrentThread(), 1 << (ProcId() % std::thread::hardware_concurrency()));
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif // _WIN32

#if defined(__GNUC__) && !defined(__APPLE__)
        int num_cores = std::thread::hardware_concurrency();
        int core_id = ProcId() % num_cores;

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);

        pthread_t current_thread = pthread_self();
        pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
#endif


        if (ProcId())
        {

#ifndef BSP_SKIP_CHECKS

            if (ProcId() == 0xdeadbeef)
            {
                fprintf(stderr, "A processor with ID `0xdeadbeef` is found, this can happen when you forget to call Init().");
            }

            assert(maxProcs == mProcCount);
            assert(ProcId() < maxProcs);
#endif

            StartTiming();
            mHistoryRecorder.InitSyncTimer(ProcId());
            return;
        }

        mAbort = false;
        mEnded = false;
        mProcCount = maxProcs;

        mProcessorsData.clear();
        mProcessorsData.resize(maxProcs);

        for (uint32_t i = 0; i < maxProcs; ++i)
        {
            ProcessorData &data = mProcessorsData[i];
            data.putRequests.resize(maxProcs);
            data.getRequests.resize(maxProcs);
            data.bufferedGetRequests.resize(maxProcs);
            data.tmpSendRequests.resize(maxProcs);
            data.tmpSendBufferStacks.resize(maxProcs);
        }

        mThreadBarrier.SetSize(maxProcs);

        mThreads.clear();
        mThreads.reserve(maxProcs);

        mHistoryRecorder.ResetResize(maxProcs);

        for (uint32_t i = 1; i < mProcCount; ++i)
        {
            mThreads.emplace_back(std::async(std::launch::async, [this](uint32_t pid)
            {
                ProcId() = pid;

                try
                {
                    mEntry();
                }
                catch (BSPInternal::BspAbort &)
                {

                }
            }, i));
        }

        StartTiming();
        mHistoryRecorder.InitSyncTimer(0);
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

        if (ProcId() == 0)
        {
            mThreads.clear();

            mProcCount = 0;
            mHistoryRecorder.PlotData();
        }
    }

    inline void SyncPoint()
    {
        mThreadBarrier.Wait(mAbort);
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
        mHistoryRecorder.RecordPreSync(pid);

        ProcessorData &data = mProcessorsData[pid];
        auto &syncBools = data.syncBools;

        CheckSyncBools(pid);

        SyncPoint();
        mHistoryRecorder.RecordProcessorsData(pid, mProcessorsData);

        bool anySync = CollectSyncBools(pid);

        if (syncBools.hasTagSizeUpdate)
        {
            //printf( "%d updates tagsize\n", pid );
            ProcessTagSizeUpdate(pid);
        }

        if (syncBools.hasGetRequests)
        {
            //printf( "%d buffers get\n", pid );
            BufferGetRequests(pid);
        }

        if (syncBools.hasTagSizeUpdate || syncBools.hasGetRequests)
        {
            //printf( "%d syncs tagsize or get\n", pid );
            SyncPoint();
        }

        if (syncBools.hasPopRequests)
        {
            //printf( "%d processes pop\n", pid );
            ProcessPopRequests(pid);
        }

        if (syncBools.hasSendRequests)
        {
            //printf( "%d processes send\n", pid );
            ProcessSendRequests(pid);
        }

        if (syncBools.hasPutRequests)
        {
            //printf( "%d processes put\n", pid );
            ProcessPutRequests(pid);
        }

        if (syncBools.hasGetRequests)
        {
            //printf( "%d processes get\n", pid );
            ProcessGetRequests(pid);
        }

        if (syncBools.hasSendRequests || syncBools.hasPopRequests || syncBools.hasPutRequests || syncBools.hasGetRequests)
        {
            //printf( "%d enters massive sync\n", pid );
            SyncPoint();
        }

        if (syncBools.hasSendRequests)
        {
            //printf( "%d clears send\n", pid );
            ClearSendRequests(pid);
        }

        if (syncBools.hasPutRequests)
        {
            //printf( "%d clears put\n", pid );
            data.putBufferStack.Clear();
        }

        if (syncBools.hasPushRequests)
        {
            //printf( "%d processes push\n", pid );
            ProcessPushRequests(pid);
            SyncPoint();
        }

        ResetBools(pid);

        if (!anySync)
        {
            SyncPoint();
        }

        mHistoryRecorder.RecordPostSync(pid);
    }

    inline void SyncPutRequests()
    {
        uint32_t &pid = ProcId();
        mHistoryRecorder.RecordPreSync(pid);

        SyncPoint();

        mHistoryRecorder.RecordProcessorsData(pid, mProcessorsData);

        ProcessPutRequests(pid);

        SyncPoint();

        mProcessorsData[pid].putBufferStack.Clear();

        mHistoryRecorder.RecordPostSync(pid);
    }

    inline void SyncGetRequests()
    {
        uint32_t &pid = ProcId();
        mHistoryRecorder.RecordPreSync(pid);

        SyncPoint();

        mHistoryRecorder.RecordProcessorsData(pid, mProcessorsData);

        BufferGetRequests(pid);

        SyncPoint();

        ProcessGetRequests(pid);

        mHistoryRecorder.RecordPostSync(pid);
    }

    inline void SyncSendRequests()
    {
        uint32_t &pid = ProcId();
        mHistoryRecorder.RecordPreSync(pid);

        SyncPoint();

        mHistoryRecorder.RecordProcessorsData(pid, mProcessorsData);

        ProcessSendRequests(pid);

        SyncPoint();

        ClearSendRequests(pid);

        mHistoryRecorder.RecordPostSync(pid);
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

    void PushReg(const void *ident, size_t size)
    {
        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert(pid < mProcCount);
        assert(mProcessorsData.size() > pid);
#endif

        BSPInternal::PushRequest &pushRequest = mProcessorsData[pid].pushRequests.InitRequest();
        pushRequest.pushRegister = ident;
        pushRequest.registerInfo.size = (uint32_t)size;
        pushRequest.registerInfo.registerCount = mProcessorsData[pid].registerCount++;
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

    void PopReg(const void *ident)
    {
        uint32_t &pid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert(pid < mProcCount);
        assert(mProcessorsData.size() > pid);
#endif

        BSPInternal::PopRequest &popRequest = mProcessorsData[pid].popRequests.InitRequest();
        popRequest.popRegister = ident;
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

    BSP_FORCEINLINE void Put(uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes)
    {
        uint32_t &tpid = ProcId();
        mHistoryRecorder.InitCommunication(tpid);
        //mHasPutRequests[mProcessorsData[tpid].syncBoolIndex] = true;

#ifndef BSP_SKIP_CHECKS
        assert(tpid < mProcCount);
        assert(pid < mProcCount);
        assert(src && dst);
#endif

        const char *srcBuff = reinterpret_cast<const char *>(src);
        const uint32_t globalId = mProcessorsData[tpid].threadRegisters.LocalToGlobal(dst);

#ifndef BSP_SKIP_CHECKS
        assert(mProcessorsData[pid].threadRegisters.GetSize() > globalId);

        /*mThreadRegisterLocation[pid][globalId]*/
        //assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        ptrdiff_t bufferLocation = mProcessorsData[tpid].putBufferStack.Alloc(nbytes, srcBuff);

        auto &putRequest = mProcessorsData[tpid].putRequests[pid].InitRequest();
        putRequest.bufferLocation = bufferLocation;
        putRequest.globalId = globalId;
        putRequest.offset = offset;
        putRequest.size = (uint32_t)nbytes;

        mHistoryRecorder.FinishCommunication(tpid);
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

    inline void Get(uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes)
    {
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert(tpid < mProcCount);
        assert(pid < mProcCount);
        assert(src && dst);
#endif

        const uint32_t globalId = mProcessorsData[tpid].threadRegisters.LocalToGlobal(
                                      src);  //mRegisters[tpid][src].registerCount;

#ifndef BSP_SKIP_CHECKS
        assert(mProcessorsData[pid].threadRegisters.GetSize() > globalId);
        //assert( mProcessorsData[pid].registers[GlobalToLocal( pid, globalId )].size >= offset + nbytes );
#endif

        //const char *srcBuff = reinterpret_cast<const char *>( GlobalToLocal( pid, globalId ) );

        BSPInternal::GetRequest &getRequest = mProcessorsData[tpid].getRequests[pid].InitRequest();
        getRequest.destination = dst;
        getRequest.globalId = globalId;
        getRequest.offset = offset;
        getRequest.size = (uint32_t)nbytes;
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

    inline void Send(uint32_t pid, const void *tag, const void *payload, const size_t size)
    {
        uint32_t &tpid = ProcId();

#ifndef BSP_SKIP_CHECKS
        assert(pid < mProcCount);
        assert(tpid < mProcCount);
#endif // !BSP_SKIP_CHECKS
        assert(mProcessorsData[tpid].newTagSize == mTagSize);

        const char *srcBuff = reinterpret_cast<const char *>(payload);
        const char *tagBuff = reinterpret_cast<const char *>(tag);

        BSPInternal::StackAllocator &tmpSendBuffer = mProcessorsData[tpid].tmpSendBufferStacks[pid];

        BSPInternal::StackAllocator::StackLocation bufferLocation = tmpSendBuffer.Alloc(size, srcBuff);
        BSPInternal::StackAllocator::StackLocation tagLocation = tmpSendBuffer.Alloc(mTagSize, tagBuff);

        BSPInternal::SendRequest &sendRequest = mProcessorsData[tpid].tmpSendRequests[pid].InitRequest();
        sendRequest.bufferLocation = bufferLocation;
        sendRequest.bufferSize = (uint32_t)size;
        sendRequest.tagLocation = tagLocation;
        sendRequest.tagSize = mTagSize;
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

    inline void Move(void *payload, size_t max_copy_size_in)
    {
        uint32_t &pid = ProcId();
        ProcessorData &data = mProcessorsData[pid];

        if (data.sendRequests.Empty() || data.sendReceivedIndex >= data.sendRequests.GetSize())
        {
            return;
        }

        assert(payload);

        BSPInternal::SendRequest &request = data.sendRequests[data.sendReceivedIndex++];

        const size_t copySize = std::min((uint32_t)max_copy_size_in, request.bufferSize);
        data.sendBufferStack.Extract(request.bufferLocation, copySize, (char *)payload);
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

    inline void SetTagsize(size_t *size)
    {
        uint32_t &pid = ProcId();
        assert(size);
        const uint32_t newSize = (uint32_t) * size;
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

    inline void GetTag(size_t *status, void *tag)
    {
        uint32_t &pid = ProcId();
        *status = (size_t) - 1;
        ProcessorData &data = mProcessorsData[pid];

        size_t index = data.sendReceivedIndex;
        const BSPInternal::RequestVector< BSPInternal::SendRequest > &sendQueue = data.sendRequests;

        if (!sendQueue.Empty() && index < sendQueue.GetSize())
        {
            const BSPInternal::SendRequest &sendRequest = sendQueue[index];
            *status = sendRequest.bufferSize;

            char *tagBuff = reinterpret_cast<char *>(tag);

#ifndef BSP_SKIP_CHECKS
            assert(sendQueue[index].tagSize == mTagSize);
#endif // !BSP_SKIP_CHECKS

            data.sendBufferStack.Extract(sendRequest.tagLocation, sendRequest.tagSize, tagBuff);
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
        : mThreadBarrier(0),
          mProcCount(0),
          mTagSize(0),
          mEnded(true),
          mAbort(false)
    {
    }

    void MarkSuperstep()
    {
        mHistoryRecorder.MarkSuperstep(ProcId());
    }

    void MarkSuperstep(uint32_t superstep)
    {
        mHistoryRecorder.MarkSuperstep(ProcId(), superstep);
    }

    void PauseRecording()
    {
        mHistoryRecorder.PauseRecording(ProcId());
    }

    void ResumeRecording()
    {
        mHistoryRecorder.ResumeRecording(ProcId());
    }

    void InitCommunication()
    {
        mHistoryRecorder.ManualInitCommunicaion(ProcId());
    }

    void FinishCommunication()
    {
        mHistoryRecorder.ManualFinishCommunicaion(ProcId());
    }

private:

    tBarrier mThreadBarrier;

    std::vector< ProcessorData > mProcessorsData;

    std::vector< std::future< void > > mThreads;

    tHistoryRecorder mHistoryRecorder;

    std::function< void() > mEntry;
    uint32_t mProcCount;
    std::atomic< uint32_t > mTagSize;

    bool mEnded;
    std::atomic_bool mAbort;

    inline void StartTiming()
    {
        assert(ProcId() != 0xdeadbeef);
        SyncPoint();
        mProcessorsData[ProcId()].startTimer.Tic();
    }

    inline void CheckAborted()
    {
        if (mAbort)
        {
            mThreadBarrier.NotifyAbort();
            throw BSPInternal::BspAbort("Aborted");
        }
    }

    inline void CheckSyncBools(uint32_t pid)
    {
        CheckHasGetRequests(pid);
        CheckHasPopRequests(pid);
        CheckHasPushRequests(pid);
        CheckHasPutRequests(pid);
        CheckHasSendRequests(pid);
        CheckHasTagSizeUpdate(pid);
    }

    inline bool CollectSyncBools(uint32_t pid)
    {
        auto &syncBools = mProcessorsData[pid].syncBools;

        for (uint32_t owner = 0; owner < mProcCount; ++owner)
        {
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

    inline void ResetBools(uint32_t pid)
    {
        auto &syncBools = mProcessorsData[pid].syncBools;
        syncBools.hasGetRequests = false;
        syncBools.hasPopRequests = false;
        syncBools.hasPushRequests = false;
        syncBools.hasPutRequests = false;
        syncBools.hasSendRequests = false;
        syncBools.hasTagSizeUpdate = false;
    }

    inline void CheckHasPushRequests(size_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasPushRequests = !data.pushRequests.Empty();
    }

    inline void ProcessPushRequests(size_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];

        if (!data.pushRequests.Empty())
        {
            for (auto pushRequest = data.pushRequests.Begin(), end = data.pushRequests.End(); pushRequest != end; ++pushRequest)
            {
                data.threadRegisters.Insert(pushRequest->pushRegister, pushRequest->registerInfo);
            }

            data.pushRequests.Clear();
        }
    }

    inline void CheckHasPutRequests(uint32_t pid)
    {
        volatile bool &hasPutRequests = mProcessorsData[pid].syncBools.hasPutRequests;
        hasPutRequests = false;

        for (size_t target = 0; !hasPutRequests && target < mProcCount; ++target)
        {
            hasPutRequests = !mProcessorsData[pid].putRequests[target].Empty();
        }
    }

    inline void ProcessPutRequests(uint32_t pid)
    {
        BSPUtil::SplitFor(0u, mProcCount, pid, [this, pid](uint32_t owner)
        {
            BSPInternal::RequestVector< BSPInternal::PutRequest > &putQueue = mProcessorsData[owner].putRequests[pid];

            if (!putQueue.Empty())
            {
                for (auto putRequest = putQueue.RBegin(), end = putQueue.REnd(); putRequest != end; ++putRequest)
                {
                    char *dstBuff = static_cast<char *>(const_cast<void *>(mProcessorsData[pid].threadRegisters.GlobalToLocal(
                                                                               putRequest->globalId))) + putRequest->offset;

                    mProcessorsData[owner].putBufferStack.Extract(putRequest->bufferLocation, putRequest->size, dstBuff);
                }

                putQueue.Clear();
            }
        });
    }

    inline void CheckHasGetRequests(uint32_t pid)
    {
        volatile bool &hasGetRequests = mProcessorsData[pid].syncBools.hasGetRequests;
        hasGetRequests = false;

        for (size_t target = 0; !hasGetRequests && target < mProcCount; ++target)
        {
            hasGetRequests = !mProcessorsData[pid].getRequests[target].Empty();
        }
    }

    inline void BufferGetRequests(uint32_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];
        data.getBufferStack.Clear();

        BSPUtil::SplitFor(0u, mProcCount, pid, [this, pid, &data](uint32_t owner)
        {
            BSPInternal::RequestVector< BSPInternal::GetRequest > &getQueue = mProcessorsData[owner].getRequests[pid];

            for (auto request = getQueue.RBegin(), end = getQueue.REnd(); request != end; ++request)
            {
                //const char *srcBuff = reinterpret_cast<const char *>( request->source );
                const char *srcBuff = reinterpret_cast<const char *>(mProcessorsData[pid].threadRegisters.GlobalToLocal(
                                                                         request->globalId)) + request->offset;

                BSPInternal::StackAllocator::StackLocation bufferLocation = data.getBufferStack.Alloc(request->size, srcBuff);

                BSPInternal::BufferedGetRequest &bufferedGetRequest = mProcessorsData[pid].bufferedGetRequests[owner].InitRequest();
                bufferedGetRequest.bufferLocation = bufferLocation;
                bufferedGetRequest.destination = request->destination;
                bufferedGetRequest.size = request->size;
            }

            getQueue.Clear();
        });
    }

    inline void ProcessGetRequests(uint32_t pid)
    {
        BSPUtil::SplitFor(0u, mProcCount, pid, [this, pid](uint32_t owner)
        {
            BSPInternal::RequestVector< BSPInternal::BufferedGetRequest > &getQueue =
                mProcessorsData[owner].bufferedGetRequests[pid];

            if (!getQueue.Empty())
            {
                for (auto getRequest = getQueue.RBegin(), end = getQueue.REnd(); getRequest != end; ++getRequest)
                {
                    char *dstBuff = static_cast< char * >(const_cast< void * >(getRequest->destination));

                    mProcessorsData[owner].getBufferStack.Extract(getRequest->bufferLocation, getRequest->size, dstBuff);
                }

                getQueue.Clear();
            }
        });
    }

    inline void CheckHasSendRequests(uint32_t pid)
    {
        volatile bool &hasSendRequests = mProcessorsData[pid].syncBools.hasSendRequests;
        hasSendRequests = false;

        for (size_t target = 0; !hasSendRequests && target < mProcCount; ++target)
        {
            hasSendRequests = !mProcessorsData[pid].tmpSendBufferStacks[target].Empty();
        }
    }



    inline void ProcessSendRequests(uint32_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];

        BSPInternal::StackAllocator::StackLocation offset = 0;
        BSPInternal::StackAllocator &sendBuffer = data.sendBufferStack;
        BSPInternal::RequestVector< BSPInternal::SendRequest > &sendQueue = data.sendRequests;
        sendQueue.Clear();
        sendBuffer.Clear();
        data.sendReceivedIndex = 0;

        BSPUtil::SplitFor(0u, mProcCount, pid, [this, pid, &sendBuffer, &sendQueue, &data, &offset](uint32_t owner)
        {
            BSPInternal::RequestVector< BSPInternal::SendRequest > &tmpQueue = mProcessorsData[owner].tmpSendRequests[pid];
            BSPInternal::StackAllocator &tmpBuffer = mProcessorsData[owner].tmpSendBufferStacks[pid];

            for (auto request = tmpQueue.Begin(), end = tmpQueue.End(); request != end; ++request)
            {
                request->bufferLocation += offset;
                request->tagLocation += offset;
            }

            offset += tmpBuffer.Size();
            sendBuffer.Merge(tmpBuffer);
            sendQueue.Append(tmpQueue.Begin(), tmpQueue.End());
        });
    }

    inline void ClearSendRequests(uint32_t pid)
    {
        for (uint32_t target = 0; target < mProcCount; ++target)
        {
            mProcessorsData[pid].tmpSendBufferStacks[target].Clear();
            mProcessorsData[pid].tmpSendRequests[target].Clear();
        }
    }

    inline void CheckHasPopRequests(size_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasPopRequests = !data.popRequests.Empty();
    }

    inline void ProcessPopRequests(size_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];

        if (!data.popRequests.Empty())
        {
            for (auto popRequest = data.popRequests.Begin(), end = data.popRequests.End(); popRequest != end; ++popRequest)
            {
                data.threadRegisters.Erase(popRequest->popRegister);
            }

            data.popRequests.Clear();
        }
    }

    inline void CheckHasTagSizeUpdate(size_t pid)
    {
        ProcessorData &data = mProcessorsData[pid];
        data.syncBools.hasTagSizeUpdate = data.newTagSize != mTagSize;
    }

    inline void ProcessTagSizeUpdate(size_t pid)
    {
        if (pid == 0)
        {
            mTagSize = mProcessorsData[0].newTagSize;
        }
    }
};

#endif