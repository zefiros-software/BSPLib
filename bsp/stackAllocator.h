#pragma once
#ifndef __STACKALLOCATOR_H__
#define __STACKALLOCATOR_H__

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
        {}

        StackAllocator( size_t size )
            : mStack( size, '@' ),
              mCursor( 0 )
        {}

        inline bool FitsInStack( size_t size )
        {
            return mCursor + size < mStack.size();
        }


        inline StackLocation Alloc( size_t size, const char *content )
        {
            if ( !FitsInStack( size ) )
            {
                Grow( size );
            }

            StackLocation loc = mCursor;
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

        inline void Grow( size_t size )
        {
            mStack.resize( static_cast<size_t>( mStack.size() * 1.6f ) + size, '~' );
        }
    };
}

#endif