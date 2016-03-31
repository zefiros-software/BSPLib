/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __BSP_MEMORYPOOLINSTANTIATOR_H__
#define __BSP_MEMORYPOOLINSTANTIATOR_H__

#include <assert.h>

namespace BspInternal
{

    template< typename tT >
    class PoolInstantiator
    {
    public:

        explicit PoolInstantiator( const size_t blocksize = 1000 )
            : mBlockSize( blocksize )
        {
            AddMemoryBlockArray();
        }


        ~PoolInstantiator()
        {
            Reset();

            for ( auto it = mMemoryBlocks.begin(), end = mMemoryBlocks.end(); it != end; ++it )
            {
                delete[] *it;
            }
        }

        tT *CreateInstance()
        {
            if ( mTotalSize > mUsed + 1 )
            {
                return GetObject();

            }

            AddMemoryBlockArray();
            return GetObject();
        }


        void Reset()
        {
            mUsed = 0;
        }

    private:

        void AddMemoryBlockArray()
        {
            mMemoryBlocks.push_back( new tT[ mBlockSize ] );
            mTotalSize += mBlockSize;
        }

        tT *GetObject()
        {
            size_t memBlock = mUsed / mBlockSize;
            size_t index = memBlock * mUsed;

            ++mUsed;

            return mMemoryBlocks[memBlock] + index;
        }

        std::vector< tT * > mMemoryBlocks;

        size_t mTotalSize;
        size_t mUsed;

        const size_t mBlockSize;
    };

}

#endif