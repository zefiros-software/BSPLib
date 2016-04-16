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
#include "helper.h"

#include <algorithm>

template< typename tBarrier >
void TestBarrierImpl( tBarrier &barrier, bool *check, const std::atomic_bool &abort, size_t id )
{
    barrier.Wait( abort );
    check[id] = true;
    barrier.Wait( abort );
}

template< typename tBarrier >
void TestBarrier( uint32_t threads, const std::atomic_bool &abort )
{
    std::vector< std::future< void > > futures;
    bool *check = new bool[threads];
    std::fill_n( check, threads, false );

    tBarrier barrier( threads );

    for ( size_t i = 0; i < threads - 1; ++i )
    {
        futures.emplace_back( std::async( std::launch::async, [&barrier, &check, &threads, &abort, i]()
        {
            try
            {
                TestBarrierImpl< tBarrier >( barrier, check, abort, i );
            }
            catch ( ... )
            {
                // Ignore other threads
            }
        } ) );

        EXPECT_EQ( 0, std::count_if( check, check + threads, []( bool b )
        {
            return b;
        } ) );
    }

    try
    {
        TestBarrierImpl< tBarrier >( barrier, check, abort, threads - 1 );
    }
    catch ( BSPInternal::BspAbort &e )
    {
        // Make sure all threads are joined, even when an exeption is thrown (mimic the behaviour of Init after Abort)
        for ( auto &thread : futures )
        {
            thread.wait_for( std::chrono::milliseconds( 200 ) );
        }

        throw e;
    }

    EXPECT_EQ( threads, std::count_if( check, check + threads, []( bool b )
    {
        return b;
    } ) );

    // Make sure all threads are joined
    for ( auto &thread : futures )
    {
        thread.wait_for( std::chrono::milliseconds( 200 ) );
    }

    delete check;
}

/*
///  Disabled since spinbarriers are not very cpu friendly
TEST( P( Barrier ), Simple2 )
{
    TestBarrier< BSPInternal::Barrier >( 2, false );
}

TEST( P( Barrier ), Simple4 )
{
    TestBarrier< BSPInternal::Barrier >( 4, false );
}

TEST( P( Barrier ), Simple8 )
{
    TestBarrier< BSPInternal::Barrier >( 8, false );
}

TEST( P( Barrier ), Simple16 )
{
    TestBarrier< BSPInternal::Barrier >( 16, false );
}

TEST( P( Barrier ), Simple32 )
{
    TestBarrier< BSPInternal::Barrier >( 32, false );
}*/

#ifndef DEBUG
TEST( P( CondVarBarrier ), Simple )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 1, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Simple2 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 2, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Simple4 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 4, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Simple8 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 8, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Simple16 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 16, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Simple32 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 32, std::atomic_bool( false ) );
}

TEST( P( MixedBarrier ), Simple2 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 2, std::atomic_bool( false ) );
}

TEST( P( MixedBarrier ), Simple4 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 4, std::atomic_bool( false ) );
}

TEST( P( MixedBarrier ), Simple8 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 8, std::atomic_bool( false ) );
}

TEST( P( MixedBarrier ), Simple16 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 16, std::atomic_bool( false ) );
}

TEST( P( MixedBarrier ), Simple32 )
{
    TestBarrier< BSPInternal::CondVarBarrier >( 32, std::atomic_bool( false ) );
}

TEST( P( CondVarBarrier ), Abort2 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::CondVarBarrier >( 2, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort4 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::CondVarBarrier >( 4, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort8 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::CondVarBarrier >( 8, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort16 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::CondVarBarrier >( 16, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort32 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::CondVarBarrier >( 32, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort2 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::MixedBarrier >( 2, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort4 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::MixedBarrier >( 4, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort8 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::MixedBarrier >( 8, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort16 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::MixedBarrier >( 16, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort32 )
{
    ASSERT_THROW( TestBarrier< BSPInternal::MixedBarrier >( 32, std::atomic_bool( true ) ), BSPInternal::BspAbort );
}

#endif // !DEBUG