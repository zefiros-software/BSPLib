#pragma once
#ifndef __CONDVARBARRIER_H__
#define __CONDVARBARRIER_H__

#include "bsp/bspAbort.h"

#include <mutex>

namespace BspInternal
{
    class CondVarBarrier
    {
    public:

        explicit CondVarBarrier( std::size_t count )
            : mCurrentCon( &mConVar1 ),
              mPreviousCon( &mConVar2 ),
              mCount( count ),
              mMax( count )
        {
        }

        void SetSize( size_t count )
        {
            mCount = count;
            mMax = count;
        }

        void Wait( std::atomic_bool &aborted )
        {
            std::unique_lock<std::mutex> lock( mMutex );

            if ( aborted )
            {
                mCurrentCon->notify_all();
                throw BspAbort( "Thread Exited" );
            }

            if ( --mCount == 0 )
            {
                Reset();

            }
            else
            {
                mCurrentCon->wait( lock );
            }
        }

    private:
        std::mutex mMutex;
        std::condition_variable mConVar1;
        std::condition_variable mConVar2;

        std::condition_variable *mCurrentCon;
        std::condition_variable *mPreviousCon;

        size_t mCount;
        size_t mMax;

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