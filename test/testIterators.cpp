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

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void PutIteratorsBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount );
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushIterator( result.begin() + tBegin, result.begin() + tEnd );

    BSPLib::Sync();

    BSPLib::PutIterator( sTarget, container.begin() + tBegin, container.begin() + tEnd, result.begin() + tBegin,
                         result.begin() + tBegin );

    BSPLib::PopIterator( result.begin() + tBegin );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )s, result[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void PutIteratorsBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount );
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushContainer( result );

    BSPLib::Sync();

    BSPLib::PutIterator( sTarget, container.begin() + tBegin, container.begin() + tEnd, result.begin(),
                         result.begin() + tBegin );

    BSPLib::PopContainer( result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )s, result[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void PutSameIteratorsBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushIterator( container.begin() + tBegin, container.begin() + tEnd );

    BSPLib::Sync();

    BSPLib::PutIterator( sTarget, container.begin() + tBegin, container.begin() + tEnd );

    BSPLib::PopIterator( container.begin() + tBegin );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )( s + 1 + i ), container[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void PutSameIteratorsBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::PutIterator( sTarget, container.begin(), container.begin() + tBegin, container.begin() + tEnd );

    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )( s + 1 + i ), container[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void PutSameIteratorsOffsetCountContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::PutIterator( sTarget, container.begin(), tBegin, tEnd - tBegin );

    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )( s + 1 + i ), container[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void GetIteratorsBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount );
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushIterator( container.begin() + tBegin, tEnd - tBegin );

    BSPLib::Sync();

    BSPLib::GetIterator( sSource, container.begin() + tBegin, container.begin() + tBegin, result.begin() + tBegin,
                         result.begin() + tEnd );

    BSPLib::PopIterator( container.begin() + tBegin );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )s, result[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void GetIteratorsBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount );
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetIterator( sSource, container.begin(), container.begin() + tBegin, result.begin() + tBegin,
                         result.begin() + tEnd );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )s, result[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void GetSameIteratorsBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetIterator( sSource, container.begin(), container.begin() + tBegin, container.begin() + tEnd );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )( s + 1 + i ), container[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
        }
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset, uint32_t tBegin, uint32_t tEnd >
void GetSameIteratorsOffsetCountContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetIterator( sSource, container.begin(), tBegin, tEnd - tBegin );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        if ( i < tBegin || i >= tEnd )
        {
            EXPECT_EQ( ( tPrimitive )( s + 1 + i ), container[i] );
        }
        else
        {
            EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
        }
    }
}

BspTest5( Iterators, 2, PutIteratorsBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, PutIteratorsBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutIteratorsBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutIteratorsBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutIteratorsBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutIteratorsBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutIteratorsBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, PutSameIteratorsBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, PutSameIteratorsBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, PutSameIteratorsOffsetCountContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsOffsetCountContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsOffsetCountContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsOffsetCountContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsOffsetCountContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsOffsetCountContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsOffsetCountContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsOffsetCountContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsOffsetCountContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsOffsetCountContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsOffsetCountContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsOffsetCountContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsOffsetCountContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsOffsetCountContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsOffsetCountContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, PutSameIteratorsOffsetCountContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, PutSameIteratorsOffsetCountContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, PutSameIteratorsOffsetCountContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, PutSameIteratorsOffsetCountContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, PutSameIteratorsOffsetCountContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, GetIteratorsBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, GetIteratorsBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetIteratorsBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetIteratorsBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetIteratorsBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetIteratorsBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetIteratorsBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, GetSameIteratorsBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Iterators, 2, GetSameIteratorsOffsetCountContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsOffsetCountContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsOffsetCountContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsOffsetCountContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsOffsetCountContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsOffsetCountContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsOffsetCountContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsOffsetCountContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsOffsetCountContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsOffsetCountContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsOffsetCountContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsOffsetCountContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsOffsetCountContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsOffsetCountContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsOffsetCountContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Iterators, 2, GetSameIteratorsOffsetCountContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Iterators, 4, GetSameIteratorsOffsetCountContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Iterators, 8, GetSameIteratorsOffsetCountContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Iterators, 16, GetSameIteratorsOffsetCountContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Iterators, 32, GetSameIteratorsOffsetCountContainerTest, uint64_t, 50, 27, 3, 39 );