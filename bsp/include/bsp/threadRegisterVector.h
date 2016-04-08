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
#ifndef __BSPLIB_THREADREGISTERVECTOR_H__
#define __BSPLIB_THREADREGISTERVECTOR_H__

#include "bsp/requests.h"

#include <vector>
#include <map>

namespace BspInternal
{
    class ThreadRegisterVector
    {
    public:

        inline size_t LocalToGlobal( const void *reg )
        {
            size_t left = 0;
            size_t right = mRegisters.size();

            while ( left < right )
            {
                size_t pivot = ( left + right ) >> 1;

                if ( mRegisters[pivot] > reg )
                {
                    right = pivot;
                }
                else if ( mRegisters[pivot] < reg )
                {
                    left = pivot;
                }
                else
                {
                    return mRegistersInfo[pivot].registerCount;
                }
            }

            return mRegistersInfo[left].registerCount;
        }

        inline const void *GlobalToLocal( size_t globalId )
        {
            return mThreadRegisterLocations[globalId];
        }

        inline void Insert( const void *reg, const BspInternal::RegisterInfo &registerInfo )
        {
            /*mRegisters[reg] = registerInfo;
            mThreadRegisterLocations.push_back( reg );*/

            if ( mRegisters.empty() )
            {
                mRegisters.push_back( reg );
                mRegistersInfo.emplace_back( registerInfo );
                mThreadRegisterLocations.push_back( reg );
                return;
            }

            size_t left = 0;
            size_t right = mRegisters.size() - 1;

            while ( left != right )
            {
                size_t pivot = ( left + right ) >> 1;

                if ( mRegisters[pivot] > reg )
                {
                    right = pivot;
                }
                else
                {
                    left = pivot;
                }
            }

            mRegisters.insert( mRegisters.begin() + left + 1, reg );
            mRegistersInfo.insert( mRegistersInfo.begin() + left + 1, registerInfo );
            mThreadRegisterLocations.push_back( reg );
        }

        inline void Erase( const void *reg )
        {
            //mThreadRegisterLocations[LocalToGlobal( reg )] = nullptr;
            //mRegisters.erase( reg );
        }

    private:

        std::vector< const void * > mRegisters;
        std::vector< RegisterInfo > mRegistersInfo;

        std::vector< const void * > mThreadRegisterLocations;
    };
}

#endif