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
#ifndef __STACKALLOCATOR_H__
#define __STACKALLOCATOR_H__

#include "util.h"

#include <vector>

namespace BspInternal
{
    class StackAllocator
    {
    public:

        typedef std::ptrdiff_t StackLocation;

        StackAllocator()
            : mStack( 10, '@' ),
              mCursor( 0 )
        {
        }

        StackAllocator( size_t size )
            : mStack( size, '@' ),
              mCursor( 0 )
        {
        }

        BSP_FORCEINLINE bool FitsInStack( size_t size )
        {
            return mCursor + size < mStack.size();
        }


        BSP_FORCEINLINE StackLocation Alloc( size_t size, const char *content )
        {
            if ( !FitsInStack( size ) )
            {
                Grow( size );
            }

            const StackLocation loc = mCursor;
            char *buffer = mStack.data() + loc;
            memcpy( buffer, content, size );

            mCursor += size;

            return loc;
        }

        inline void Extract( StackLocation location, size_t size, char *dst ) const
        {
            memcpy( dst, mStack.data() + location, size );
        }

        inline void Clear()
        {
            mCursor = 0;
        }

        inline void MoveBack( StackAllocator &sa )
        {
            Alloc( sa.mCursor, sa.mStack.data() );

            sa.Clear();
        }

        inline StackLocation Size() const
        {
            return mCursor;
        }

    private:

        std::vector< char > mStack;
        StackLocation mCursor;

        BSP_FORCEINLINE void Grow( size_t size )
        {
            mStack.resize( static_cast<size_t>( mStack.size() * 1.6f ) + size, '~' );
        }
    };
}

#endif