#pragma once
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <atomic>

namespace BspInternal
{
    class SpinLock
    {
    public:

        SpinLock()
        {
        }

        void lock()
        {
            while ( mLockValue.test_and_set( std::memory_order_acquire ) )
            {
            }
        }

        bool try_lock()
        {
            return !mLockValue.test_and_set( std::memory_order_acquire );
        }

        void unlock()
        {
            mLockValue.clear( std::memory_order_release );
        }

    private:

        std::atomic_flag mLockValue;

        SpinLock( const SpinLock & );
    };
}

#endif