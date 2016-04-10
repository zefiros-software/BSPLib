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
#ifndef __BSPLIB_REQUESTVECTOR_H__
#define __BSPLIB_REQUESTVECTOR_H__

#include "bsp/requests.h"

#include <algorithm>
#include <vector>
#include <map>

namespace BspInternal
{
    template< typename tRequest >
    class RequestVector
    {
    public:
    
        RequestVector()
            : mRequests( 10 ),
              mCursor( 0 ),
              mSize(10)
        {
        }

        /**
         * Constructor.
         *
         * @param   size The number of requests.
         */

        RequestVector( size_t size )
            : mRequests( size ),
              mCursor( 0 ),
              mSize(size)
        {
        }
        
        tRequest &GetRequest(size_t index)
        {
            return mRequests[index];
        }
        
        tRequest &InitRequest()
        {
            if(mCursor >= mSize )
            {
                Grow();
            }
            
            return mRequests[mCursor++];
        }
        
        size_t GetSize()
        {
            return mCursor;
        }
        
        void Clear()
        {
            mCursor = 0;
        }
        
        bool Empty()
        {
            return mCursor == 0;
        }
        
        typename std::vector< tRequest >::reverse_iterator RBegin()
        {
            return mRequests.rbegin();
        }
        
        typename std::vector< tRequest >::reverse_iterator REnd()
        {
            return mRequests.rend();
        }

    private:
    
        std::vector< tRequest > mRequests;
        
        size_t mCursor;
        size_t mSize;
        
        void Grow()
        {
            mRequests.resize( static_cast<size_t>( mSize * 1.6f ) + 1 );
            mSize = mRequests.size();
        }
    };
}

#endif