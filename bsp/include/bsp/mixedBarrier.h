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
#ifndef __MIXEDBARRIER_H__
#define __MIXEDBARRIER_H__

#define  SPIN_ITERATIONS 2500

#include "bsp/bspAbort.h"
#include "bsp/spinLock.h"

#include <mutex>

namespace BspInternal
{
    class MixedBarrier
    {
    public:

        explicit MixedBarrier( uint32_t count )
            : mCurrentCon( &mConVar1 ),
              mPreviousCon( &mConVar2 ),
              mCount( count ),
              mMax( count ),
              mSpaces( count )
        {
        }

        void SetSize( uint32_t count )
        {
            mCount = count;
            mMax = count;
            mSpaces = count;
        }

        void Wait( std::atomic_bool &aborted )
        {
            const uint32_t myGeneration = mGeneration;

            if ( aborted )
            {
                mCurrentCon->notify_all();
                throw BspAbort( "Thread Exited" );
            }

            if ( !--mSpaces )
            {
                mSpaces = mMax;
                std::lock_guard< std::mutex > condVarLoc( mCondVarMutex );
                ++mGeneration;

                Reset();
            }
            else
            {
                volatile size_t i = 0;

                while ( mGeneration == myGeneration && ++i < SPIN_ITERATIONS )
                {
                    if ( aborted )
                    {
                        throw BspAbort( "Thread Exited" );
                    }
                }



                if ( i >= SPIN_ITERATIONS )
                {
                    std::unique_lock< std::mutex > condVarLoc( mCondVarMutex );

                    if ( mGeneration == myGeneration )
                    {
                        mCurrentCon->wait( condVarLoc );
                    }
                }
            }
        }

    private:

        std::mutex mCondVarMutex;
        std::condition_variable mConVar1;
        std::condition_variable mConVar2;

        std::condition_variable *mCurrentCon;
        std::condition_variable *mPreviousCon;

        uint32_t mCount;
        uint32_t mMax;

        std::atomic_uint_fast32_t mSpaces;
        std::atomic_uint_fast32_t mGeneration;

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