#pragma once
#ifndef __BARRIER_H__
#define __BARRIER_H__

#include "bspAbort.h"

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