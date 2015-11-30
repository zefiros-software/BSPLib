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

template< typename tBarrier >
void TestBarrierImpl( tBarrier &barrier, std::vector< uint8_t > &checks, std::atomic_bool &abort,
                      size_t id )
{
    barrier.Wait( abort );
    checks[id] = 1;
    barrier.Wait( abort );
}

template< typename tBarrier >
void TestBarrier( uint32_t threads, std::atomic_bool abort )
{
    std::vector< std::future< void > > futures;
    std::vector< uint8_t > check( threads );
    tBarrier barrier( threads );

    for ( size_t i = 0; i < threads - 1; ++i )
    {
        futures.emplace_back( std::async( std::launch::async, [&barrier, &check, &threads, &abort, i]()
        {
            TestBarrierImpl< tBarrier >( barrier, check, abort, i );
        } ) );

        EXPECT_EQ( 0, std::count_if( check.begin(), check.end(), []( uint8_t b )
        {
            return b > 0;
        } ) );
    }

    TestBarrierImpl< tBarrier >( barrier, check, abort, threads - 1 );

    EXPECT_EQ( threads, std::count_if( check.begin(), check.end(), []( uint8_t b )
    {
        return b > 0;
    } ) );
}

/*
TEST( P( Barrier ), Simple2 )
{
    TestBarrier< BspInternal::Barrier >( 2, false );
}

TEST( P( Barrier ), Simple4 )
{
    TestBarrier< BspInternal::Barrier >( 4, false );
}

TEST( P( Barrier ), Simple8 )
{
    TestBarrier< BspInternal::Barrier >( 8, false );
}

TEST( P( Barrier ), Simple16 )
{
    TestBarrier< BspInternal::Barrier >( 16, false );
}

TEST( P( Barrier ), Simple32 )
{
    TestBarrier< BspInternal::Barrier >( 32, false );
}*/

#ifndef DEBUG
TEST( P( CondVarBarrier ), Simple )
{
    TestBarrier< BspInternal::CondVarBarrier >( 1, false );
}

TEST( P( CondVarBarrier ), Simple2 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 2, false );
}

TEST( P( CondVarBarrier ), Simple4 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 4, false );
}

TEST( P( CondVarBarrier ), Simple8 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 8, false );
}

TEST( P( CondVarBarrier ), Simple16 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 16, false );
}

TEST( P( CondVarBarrier ), Simple32 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 32, false );
}

TEST( P( MixedBarrier ), Simple2 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 2, false );
}

TEST( P( MixedBarrier ), Simple4 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 4, false );
}

TEST( P( MixedBarrier ), Simple8 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 8, false );
}

TEST( P( MixedBarrier ), Simple16 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 16, false );
}

TEST( P( MixedBarrier ), Simple32 )
{
    TestBarrier< BspInternal::CondVarBarrier >( 32, false );
}

TEST( P( CondVarBarrier ), Abort2 )
{
    ASSERT_THROW( TestBarrier< BspInternal::CondVarBarrier >( 2, true ), BspInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort4 )
{
    ASSERT_THROW( TestBarrier< BspInternal::CondVarBarrier >( 4, true ), BspInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort8 )
{
    ASSERT_THROW( TestBarrier< BspInternal::CondVarBarrier >( 8, true ), BspInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort16 )
{
    ASSERT_THROW( TestBarrier< BspInternal::CondVarBarrier >( 16, true ), BspInternal::BspAbort );
}

TEST( P( CondVarBarrier ), Abort32 )
{
    ASSERT_THROW( TestBarrier< BspInternal::CondVarBarrier >( 32, true ), BspInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort2 )
{
    ASSERT_THROW( TestBarrier< BspInternal::MixedBarrier >( 2, true ), BspInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort4 )
{
    ASSERT_THROW( TestBarrier< BspInternal::MixedBarrier >( 4, true ), BspInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort8 )
{
    ASSERT_THROW( TestBarrier< BspInternal::MixedBarrier >( 8, true ), BspInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort16 )
{
    ASSERT_THROW( TestBarrier< BspInternal::MixedBarrier >( 16, true ), BspInternal::BspAbort );
}

TEST( P( MixedBarrier ), Abort32 )
{
    ASSERT_THROW( TestBarrier< BspInternal::MixedBarrier >( 32, true ), BspInternal::BspAbort );
}

#endif // !DEBUG