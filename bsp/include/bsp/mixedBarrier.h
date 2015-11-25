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