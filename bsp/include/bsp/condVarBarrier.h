/**
 * Copyright (c) 2017 Zefiros Software.
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
#ifndef __BSPLIB_CONDVARBARRIER_H__
#define __BSPLIB_CONDVARBARRIER_H__

#include "bsp/bspAbort.h"

#include <condition_variable>
#include <atomic>
#include <mutex>

namespace BSPInternal
{
    /**
     * A condition variable barrier implementation. This barrier
     * does not use busy waiting, but relies on events in stead.
     * The advantage of this approach is that is more reliable,
     * however there is a lot of overhead.
     */

    class CondVarBarrier
    {
    public:

        /**
         * Constructor.
         *
         * @param   count Number of threads to wait for.
         */

        explicit CondVarBarrier( std::size_t count )
            : mCurrentCon( &mConVar1 ),
              mPreviousCon( &mConVar2 ),
              mCount( count ),
              mMax( count )
        {
        }

        /**
         * Sets the size of the barrier, thus the number of threads to wait for on a sync point.
         *
         * @param   count Number of threads to wait on.
         *
         * @post The amount of threads the barriers waits on equals count.
         */

        void SetSize( size_t count )
        {
            mCount = count;
            mMax = count;
        }

        /**
         * Waits for all the threads to reach the sync point, however the process can be aborted when `aborted` equals to
         * true.
         *
         * @param [in,out]  aborted Check whether the process should be aborted.
         *
         * @pre if aborted == true, all threads quit computations.
         *
         * @post all threads have waited for each other to reach the barrier.
         */

        void Wait( const std::atomic_bool &aborted )
        {
            std::unique_lock<std::mutex> lock( mMutex );

            if ( aborted )
            {
                mCurrentCon->notify_all();
                throw BspAbort( "Aborted" );
            }

            if ( --mCount == 0 )
            {
                Reset();
            }
            else
            {
                mCurrentCon->wait( lock );
            }
        }

    private:

        std::mutex mMutex;
        std::condition_variable mConVar1;
        std::condition_variable mConVar2;

        std::condition_variable *mCurrentCon;
        std::condition_variable *mPreviousCon;

        size_t mCount;
        size_t mMax;

        /**
         * Resets the barrier for reuse.
         */

        void Reset()
        {
            mCount = mMax;
            std::condition_variable *tmpCon = mCurrentCon;
            mCurrentCon = mPreviousCon;
            mPreviousCon = tmpCon;

            tmpCon->notify_all();
        }
    };
}

#endif