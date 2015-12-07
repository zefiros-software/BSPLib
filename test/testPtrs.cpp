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
void PutPointersBeginEndOffsetTest()
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
void GetPointersBeginEndOffsetTest()
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

BspTest5( Pointers, 2, PutPointersBeginEndOffsetTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndOffsetTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndOffsetTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndOffsetTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndOffsetTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndOffsetTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndOffsetTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndOffsetTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndOffsetTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndOffsetTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndOffsetTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndOffsetTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndOffsetTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndOffsetTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndOffsetTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, PutPointersBeginEndOffsetTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, PutPointersBeginEndOffsetTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, PutPointersBeginEndOffsetTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, PutPointersBeginEndOffsetTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, PutPointersBeginEndOffsetTest, uint64_t, 50, 27, 3, 39 );

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

BspTest5( Pointers, 2, GetPointersBeginEndOffsetTest, uint8_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndOffsetTest, uint8_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndOffsetTest, uint8_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndOffsetTest, uint8_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndOffsetTest, uint8_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndOffsetTest, uint16_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndOffsetTest, uint16_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndOffsetTest, uint16_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndOffsetTest, uint16_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndOffsetTest, uint16_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndOffsetTest, uint32_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndOffsetTest, uint32_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndOffsetTest, uint32_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndOffsetTest, uint32_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndOffsetTest, uint32_t, 50, 27, 3, 39 );
BspTest5( Pointers, 2, GetPointersBeginEndOffsetTest, uint64_t, 50, 1, 13, 29 );
BspTest5( Pointers, 4, GetPointersBeginEndOffsetTest, uint64_t, 50, 3, 17, 43 );
BspTest5( Pointers, 8, GetPointersBeginEndOffsetTest, uint64_t, 50, 7, 19, 47 );
BspTest5( Pointers, 16, GetPointersBeginEndOffsetTest, uint64_t, 50, 5, 7, 31 );
BspTest5( Pointers, 32, GetPointersBeginEndOffsetTest, uint64_t, 50, 27, 3, 39 );