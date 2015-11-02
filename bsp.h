#pragma once
#ifndef __BSP_H__
#define __BSP_H__

#include <condition_variable>
#include <unordered_map>
#include <assert.h>
#include <thread>
#include <future>
#include <mutex>

thread_local static size_t gPID = 0;

void main( uint32_t argc, char **argv );

class BSP
{
public:

    BSP()
        : mAbort( false ),
          mEnded( true ),
          mThreadBarrier( 0 )
    {
    }

    void Abort( const char *format, ... )
    {
        va_list args;
        va_start( args, format );

        //pass to bsp_vabort
        VAbort( format, args );

        //mark end of variable arguments
        va_end( args );
    }

    size_t NProcs() const
    {
        return std::thread::hardware_concurrency();
    }

    size_t PID() const
    {
        return gPID;
    }

    double Time()
    {
        CheckAborted();

        const std::chrono::time_point< std::chrono::high_resolution_clock > now =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - mStartTime;
        return diff.count();
    }

    void Init( std::function< void() > entry, uint32_t argc, char *argv[] )
    {
        mEntry = entry;

        if ( !mEnded )
        {
            fprintf( stderr, "Warning: initialisation data corresponding to another BSP run found;\n" );
            fprintf( stderr, "         and this other run did not terminate (gracefully).\n" );
        }
    }

    void Begin( size_t maxProcs )
    {
        if ( mThreads.size() > 0 )
        {
            return;
        }

        mAbort = false;
        mEnded = true;

        mRegisters.clear();
        mRegisters.resize( maxProcs );

        mRegisterCount.clear();
        mRegisterCount.resize( maxProcs );

        mThreadRegisterLocation.clear();
        mThreadRegisterLocation.resize( maxProcs );

        mPutRequests.clear();
        mPutRequests.resize( maxProcs );

        for ( auto &requests : mPutRequests )
        {
            requests.resize( maxProcs );
        }

        mGetRequests.clear();
        mGetRequests.resize( maxProcs );

        for ( auto &requests : mGetRequests )
        {
            requests.resize( maxProcs );
        }

        mThreadBarrier.SetSize( maxProcs );
        mProcCount = maxProcs;

        StartTiming();

        mThreads.reserve( maxProcs );

        for ( size_t i = 1, end = mProcCount; i < end; ++i )
        {
            mThreads.emplace_back( std::async( std::launch::async, [this]( uint32_t pid )
            {
                gPID = pid;

                if ( mEntry )
                {
                    mEntry();
                }
                else
                {
                    main( 0, nullptr );
                }
            }, i ) );
        }
    }

    void End()
    {
        mEnded = true;

        SyncPoint();

        if ( gPID == 0 )
        {
            mThreads.clear();
        }
        else
        {
            std::terminate();
        }
    }

    void SyncPoint()
    {
        SyncPoint();
    }

    void Sync()
    {
        mThreadBarrier.Wait( &mAbort );

        for ( auto &puts : mPutRequests )
        {

        }
    }

    void PushReg( const void *ident, size_t size )
    {
        CheckAborted();

        assert( gPID >= 0 && gPID < mProcCount );
        assert( mRegisters.size() > gPID );
        assert( mRegisterCount.size() > gPID );
        assert( mThreadRegisterLocation.size() > gPID );

        mRegisters[gPID][ident] = { size, mRegisterCount[gPID]++ };
        mThreadRegisterLocation[gPID].push_back( ident );
    }

    void PopReg( const void *ident )
    {
        CheckAborted();

        assert( gPID >= 0 && gPID < mProcCount );
        assert( mRegisters.size() > gPID );
        assert( mRegisterCount.size() > gPID );
        assert( mThreadRegisterLocation.size() > gPID );

        mRegisters[gPID].erase( ident );
    }

    void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {
        assert( pid < mProcCount );
        assert( gPID < mProcCount );

        const char *srcBuff = reinterpret_cast<const char *>( src );
        const size_t globalId = mRegisters[gPID][dst].registerCount;

        assert( mRegisters[gPID][mThreadRegisterLocation[pid][globalId]].size >= nbytes );

        const char *dstBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );
        mPutRequests[gPID][pid].emplace_back( PutRequest{ std::vector< char >( srcBuff, srcBuff + nbytes ), dstBuff + offset } );
    }

    void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {
        assert( pid < mProcCount );
        assert( gPID < mProcCount );

        const size_t globalId = mRegisters[gPID][src].registerCount;

        assert( mRegisters[gPID][mThreadRegisterLocation[pid][globalId]].size >= nbytes );

        const char *srcBuff = reinterpret_cast<const char *>( mThreadRegisterLocation[pid][globalId] );
        mPutRequests[gPID][pid].emplace_back( GetRequest{ dst, srcBuff + offset, nbytes } );
    }

    void HPPut( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
    {

    }

    void HPGet( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
    {

    }

    size_t HPMove( void **tag_ptr, void **payload_ptr )
    {

    }

private:

    class Barrier
    {
    public:

        explicit Barrier( std::size_t count )
            : mCount( count ),
              mMax( count )
        {
        }

        void SetSize( size_t count )
        {
            mCount = count;
            mMax = count;
        }

        void Wait( std::atomic_bool *aborted )
        {
            std::unique_lock<std::mutex> lock( mMutex );

            if ( aborted && *aborted )
            {
                mConVar.notify_all();
                std::terminate();
            }

            if ( --mCount == 0 )
            {
                mConVar.notify_all();
            }
            else
            {
                mConVar.wait( lock, [this]
                {
                    return mCount == 0;
                } );
            }

            Reset();
        }

        void Reset()
        {
            mCount = mMax;
        }

    private:
        std::mutex mMutex;
        std::condition_variable mConVar;
        std::size_t mCount;
        std::size_t mMax;
    };

    struct RegisterInfo
    {
        size_t size;
        size_t registerCount;
    };

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

    struct PutRequest
    {
        std::vector< char > buffer;
        const void *destination;
    };

    struct GetRequest
    {
        const void *destination;
        const void *source;
        size_t size;
    };

    Barrier mThreadBarrier;

    std::vector< std::vector< std::vector< PutRequest > > > mPutRequests;
    std::vector< std::vector< std::vector< GetRequest > > > mGetRequests;

    std::vector< size_t > mRegisterCount;
    std::vector<std::unordered_map< const void *, RegisterInfo > > mRegisters;
    std::vector< std::vector< const void * > > mThreadRegisterLocation;

    std::vector< std::future< void > > mThreads;
    std::function< void() > mEntry;
    std::chrono::time_point< std::chrono::high_resolution_clock > mStartTime;
    size_t mProcCount;

    bool mEnded;
    std::atomic_bool mAbort;

    void StartTiming()
    {
        mStartTime = std::chrono::high_resolution_clock::now();
    }

    void CheckAborted()
    {
        if ( mAbort )
        {
            std::terminate();
        }
    }

    void VAbort( const char *format, va_list args )
    {
        mAbort = true;
        vfprintf( stderr, format, args );
        CheckAborted();
    }

};

void bsp_abort( const char *format, ... )
{

}

size_t bsp_nprocs()
{

}

size_t bsp_pid()
{

}

double bsp_time()
{

}

void bsp_sync()
{

}

void bsp_push_reg( const void *ident, size_t size )
{


}

void bsp_pop_reg( const void *ident )
{

}

void bsp_put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{

}

void bsp_get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{

}

void bsp_hpput( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{

}

void bsp_hpget( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{

}

size_t bsp_hpmove( void **tag_ptr, void **payload_ptr )
{

}

#endif