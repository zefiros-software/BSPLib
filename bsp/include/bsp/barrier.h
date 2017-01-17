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
#ifndef __BSPLIB_BARRIER_H__
#define __BSPLIB_BARRIER_H__

#include "bsp/bspAbort.h"

namespace BSPInternal
{
    /**
     * A lock free barrier implementation. It spins until all threads reach the barrier.
     * This barrier uses busy waiting, and thus the performance is unstable, but fast.
     */

    class Barrier
    {
    public:

        /**
         * Constructor.
         *
         * @param   count Number of threads to wait for.
         */

        explicit Barrier( uint32_t count ) :
            mCount( count ),
            mSpaces( count ),
            mGeneration( 0 )
        {
        }

        /**
         * Sets the size of the barrier, thus the number of threads to wait for on a sync point.
         *
         * @param   count Number of threads to wait on.
         *
         * @post The amount of threads the barriers waits on equals count.
         */

        void SetSize( uint32_t count )
        {
            mCount = count;
            mSpaces = count;
            mGeneration = 0;
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
                    if ( aborted )
                    {
                        throw BspAbort( "Aborted" );
                    }
                }
            }
        }

        void NotifyAbort()
        {
            ++mGeneration;
        }

    private:

        /// The amount of threads to wait for in total
        uint32_t mCount;

        /// The amount of threads filling the barrier currently
        std::atomic_uint_fast32_t mSpaces;

        /// The current waiting generation, so we can reuse the barrier
        std::atomic_uint_fast32_t mGeneration;
    };
}

#endif