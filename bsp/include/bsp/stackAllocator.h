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
#ifndef __BSPLIB_STACKALLOCATOR_H__
#define __BSPLIB_STACKALLOCATOR_H__

#include "util.h"

#include <cstring>
#include <cstddef>
#include <vector>

namespace BSPInternal
{
    /**
     * A stack allocator implementation, that will allocate memory in contiguous memory on the stack,
     * optimising cache line efficiency.
     */

    class StackAllocator
    {
    public:

        typedef std::ptrdiff_t StackLocation;

        /**
         * Default constructor. Starts with byte size 10.
         */

        StackAllocator()
            : mStack( 10, '@' ),
              mCursor( 0 ),
              mSize( 10 )
        {
        }

        /**
         * Constructor.
         *
         * @param   size The size in bytes.
         */

        StackAllocator( size_t size )
            : mStack( size, '@' ),
              mCursor( 0 ),
              mSize( size )
        {
        }

        /**
         * Check whether an object of given amount of its in stack.
         *
         * @param   size The size of the object in bytes.
         *
         * @return true if it will succeed, false if it fails.
         */

        BSP_FORCEINLINE bool FitsInStack( size_t size ) const
        {
            return mCursor + size < mSize;
        }

        /**
         * Allocates the given amount of bytes on the stack. With content as pointer to
         * what we want to allocate on the stack. (due to resizing invalidation we cannot return a pointer)
         *
         * @param   size    The size of the content in bytes.
         * @param   content The content to place on the stack.
         *
         * @return A StackLocation that refers to the object.
         */

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

        /**
         * Extracts this object on the given stack location.
         *
         * @param   location    The location to read the memory from.
         * @param   size        The size in bytes.
         * @param [in,out]  dst If non-null, destination to read the object to.
         */

        inline void Extract( StackLocation location, size_t size, char *dst ) const
        {
            memcpy( dst, mStack.data() + location, size );
        }

        /**
         * Clears this object to its blank/initial state.
         */

        inline void Clear()
        {
            mCursor = 0;
        }

        /**
         * Merges two stackallocator to one large stack allocator.
         *
         * @param [in,out]  sa The stack allocator.
         */

        inline void Merge( StackAllocator &sa )
        {
            Alloc( sa.mCursor, sa.mStack.data() );
        }

        /**
         * Gets the size of the stack.
         *
         * @return A StackLocation.
         */

        inline StackLocation Size() const
        {
            return mCursor;
        }

        inline bool Empty() const
        {
            return mCursor == 0;
        }

    private:

        /// The stack buffer
        std::vector< char > mStack;
        /// The current size of the stack
        StackLocation mCursor;
        size_t mSize;

        /**
         * Grows the stack with a rate of phi, which is mathematically the most efficient
         * growing rate. [See also](https://crntaylor.wordpress.com/2011/07/15/optimal-memory-reallocation-and-the-golden-ratio/).
         *
         * @param   size The size to at least be able to allocate.
         */

        BSP_FORCEINLINE void Grow( size_t size )
        {
            mStack.resize( static_cast<size_t>( mSize * 1.6f ) + size, '~' );
            mSize = mStack.size();
        }
    };
}

#endif