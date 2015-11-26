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
#ifndef __BARRIER_H__
#define __BARRIER_H__

#include "bsp/bspAbort.h"

namespace BspInternal
{
    class Barrier
    {
    public:

        Barrier( uint32_t count ) :
            mCount( count ),
            mSpaces( count ),
            mGeneration( 0 )
        {
        }

        void SetSize( uint32_t count )
        {
            mCount = count;
            mSpaces = count;
            mGeneration = 0;
        }

        void Wait( std::atomic_bool &aborted )
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
                        throw BspAbort( "Thread Exited" );
                    }
                }
            }
        }

    private:

        uint32_t mCount;
        std::atomic_uint_fast32_t mSpaces;
        std::atomic_uint_fast32_t mGeneration;
    };
}

#endif