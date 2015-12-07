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
void PutPointersBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    tPrimitive result[tCount];
    std::fill_n( result, tCount, ( tPrimitive )s );

    BSPLib::PushPtrs( result + tBegin, result + tEnd );

    BSPLib::Sync();

    BSPLib::PutPtrs( sTarget, container + tBegin, container + tEnd, result + tBegin, result + tBegin );
    BSPLib::PopPtrs( result + tBegin );

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
void PutPointersBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    tPrimitive result[tCount];
    std::fill_n( result, tCount, ( tPrimitive )s );

    BSPLib::PushCArray( result );

    BSPLib::Sync();

    BSPLib::PutPtrs( sTarget, container + tBegin, container + tEnd, result, result + tBegin );

    BSPLib::PopCArray( result );

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
void PutSamePointersBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushPtrs( container + tBegin, container + tEnd );

    BSPLib::Sync();

    BSPLib::PutPtrs( sTarget, container + tBegin, container + tEnd );

    BSPLib::PopPtrs( container + tBegin );

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
void PutSamePointersBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushCArray( container );

    BSPLib::Sync();

    BSPLib::PutPtrs( sTarget, container, container + tBegin, container + tEnd );

    BSPLib::PopPtrs( container + tBegin );

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
void PutSamePointersOffsetCountContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushCArray( container );

    BSPLib::Sync();

    BSPLib::PutPtrs( sTarget, container, tBegin, tEnd - tBegin );

    BSPLib::PopCArray( container );

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
void GetPointersBeginEndTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    tPrimitive result[tCount];
    std::fill_n( result, tCount, ( tPrimitive )s );

    BSPLib::PushPtrs( container + tBegin, tEnd - tBegin );

    BSPLib::Sync();

    BSPLib::GetPtrs( sSource, container + tBegin, container + tBegin, result + tBegin, result + tEnd );
    BSPLib::PopPtrs( container + tBegin );

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
void GetPointersBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    tPrimitive result[tCount];
    std::fill_n( result, tCount, ( tPrimitive )s );

    BSPLib::PushCArray( container );

    BSPLib::Sync();

    BSPLib::GetPtrs( sSource, container, container + tBegin, result + tBegin, result + tEnd );
    BSPLib::PopCArray( container );

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
void GetSamePointersBeginEndContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushCArray( container );

    BSPLib::Sync();

    BSPLib::GetPtrs( sSource, container, container + tBegin, container + tEnd );
    BSPLib::PopCArray( container );

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
void GetSamePointersOffsetCountContainerTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive container[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::PushCArray( container );

    BSPLib::Sync();

    BSPLib::GetPtrs( sSource, container, tBegin, tEnd - tBegin );
    BSPLib::PopCArray( container );

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



BspTest5( Pointers, 2, PutPointersBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, PutSamePointersBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, PutSamePointersBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, PutSamePointersOffsetCountContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersOffsetCountContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersOffsetCountContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersOffsetCountContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersOffsetCountContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersOffsetCountContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersOffsetCountContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersOffsetCountContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersOffsetCountContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersOffsetCountContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersOffsetCountContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersOffsetCountContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersOffsetCountContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersOffsetCountContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersOffsetCountContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutSamePointersOffsetCountContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutSamePointersOffsetCountContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutSamePointersOffsetCountContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutSamePointersOffsetCountContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutSamePointersOffsetCountContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, GetPointersBeginEndTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, GetPointersBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, GetSamePointersBeginEndContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersBeginEndContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersBeginEndContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersBeginEndContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersBeginEndContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersBeginEndContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersBeginEndContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersBeginEndContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersBeginEndContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersBeginEndContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersBeginEndContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersBeginEndContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersBeginEndContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersBeginEndContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersBeginEndContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersBeginEndContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersBeginEndContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersBeginEndContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersBeginEndContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersBeginEndContainerTest, uint64_t, 50, 27, 3, 39 );

BspTest5( Pointers, 2, GetSamePointersOffsetCountContainerTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersOffsetCountContainerTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersOffsetCountContainerTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersOffsetCountContainerTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersOffsetCountContainerTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersOffsetCountContainerTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersOffsetCountContainerTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersOffsetCountContainerTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersOffsetCountContainerTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersOffsetCountContainerTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersOffsetCountContainerTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersOffsetCountContainerTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersOffsetCountContainerTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersOffsetCountContainerTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersOffsetCountContainerTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetSamePointersOffsetCountContainerTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetSamePointersOffsetCountContainerTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetSamePointersOffsetCountContainerTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetSamePointersOffsetCountContainerTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetSamePointersOffsetCountContainerTest, uint64_t, 50, 27, 3, 39 );