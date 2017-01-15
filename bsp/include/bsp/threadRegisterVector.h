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
#ifndef __BSPLIB_THREADREGISTERVECTOR_H__
#define __BSPLIB_THREADREGISTERVECTOR_H__

#include "bsp/requests.h"

#include <algorithm>
#include <vector>
#include <map>

namespace BSPInternal
{
    class ThreadRegisterVector
    {
    public:

        ThreadRegisterVector()
            : mRegisterCache( nullptr ),
              mLocationCache( ( uint32_t )( -1 ) )
        {

        }

        BSP_FORCEINLINE uint32_t LocalToGlobal( const void *reg )
        {
            if ( reg != mRegisterCache )
            {
                auto l = std::lower_bound( mRegisters.begin(), mRegisters.end(), reg );

                mRegisterCache = reg;
                mLocationCache = mRegistersInfo[l - mRegisters.begin()].registerCount;
            }

            return mLocationCache;
        }

        BSP_FORCEINLINE const void *GlobalToLocal( uint32_t globalId )
        {
            if ( mLocationCache == globalId )
            {
                return mRegisterCache;
            }

            mLocationCache = globalId;
            mRegisterCache = mThreadRegisterLocations[globalId];

            return mRegisterCache;
        }

        inline void Insert( const void *reg, const BSPInternal::RegisterInfo &registerInfo )
        {
            if ( mRegisters.empty() )
            {
                mRegisters.push_back( reg );
                mRegistersInfo.emplace_back( registerInfo );
                mThreadRegisterLocations.push_back( reg );
                return;
            }

            auto l = std::lower_bound( mRegisters.begin(), mRegisters.end(), reg );

            if ( l != mRegisters.end() && *l == reg )
            {
                mRegistersInfo[l - mRegisters.begin()] = registerInfo;
                mThreadRegisterLocations.push_back( reg );
            }
            else
            {
                mRegistersInfo.insert( mRegistersInfo.begin() + ( l - mRegisters.begin() ), registerInfo );
                mRegisters.insert( l, reg );
                mThreadRegisterLocations.push_back( reg );
            }
        }

        inline void Erase( const void *reg )
        {
            auto regIt = std::find( mRegisters.begin(), mRegisters.end(), reg );

            if ( regIt != mRegisters.end() )
            {
                mRegistersInfo.erase( mRegistersInfo.begin() + ( regIt - mRegisters.begin() ) );
                mRegisters.erase( regIt );
            }
        }

        size_t GetSize() const
        {
            return mThreadRegisterLocations.size();
        }

    private:

        std::vector< const void * > mRegisters;
        std::vector< RegisterInfo > mRegistersInfo;

        std::vector< const void * > mThreadRegisterLocations;

        const void *mRegisterCache;
        uint32_t mLocationCache;
    };
}

#endif