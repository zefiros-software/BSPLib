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
#pragma once
#ifndef __TESTCONTAINER_H__
#define __TESTCONTAINER_H__

#include "helper.h"

#include <array>

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutVectorTest()
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

    std::vector< tPrimitive > result( tCount, ( tPrimitive )s );

    BSPLib::PushRegContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutFPVectorTest()
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

    std::vector< tPrimitive > result( tCount, ( tPrimitive )s );

    BSPLib::PushRegContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > container;

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::array< tPrimitive, tCount > result;
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushRegContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutFPStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > container;

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::array< tPrimitive, tCount > result;
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushRegContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutCArrayTest()
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

    BSPLib::PushRegCArray( result );

    BSPLib::Sync();

    BSPLib::PutCArray( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutFPCArrayTest()
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

    BSPLib::PushRegCArray( result );

    BSPLib::Sync();

    BSPLib::PutCArray( sTarget, container, result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

BspTest3( Container, 2, PutVectorTest, uint8_t, 20, 1 );
BspTest3( Container, 4, PutVectorTest, uint8_t, 20, 3 );
BspTest3( Container, 8, PutVectorTest, uint8_t, 20, 7 );
BspTest3( Container, 16, PutVectorTest, uint8_t, 20, 5 );
BspTest3( Container, 32, PutVectorTest, uint8_t, 20, 27 );
BspTest3( Container, 2, PutVectorTest, uint8_t, 50, 1 );
BspTest3( Container, 4, PutVectorTest, uint8_t, 50, 3 );
BspTest3( Container, 8, PutVectorTest, uint8_t, 50, 7 );
BspTest3( Container, 16, PutVectorTest, uint8_t, 50, 5 );
BspTest3( Container, 32, PutVectorTest, uint8_t, 50, 27 );

BspTest3( Container, 2, PutVectorTest, uint64_t, 20, 1 );
BspTest3( Container, 4, PutVectorTest, uint64_t, 20, 3 );
BspTest3( Container, 8, PutVectorTest, uint64_t, 20, 7 );
BspTest3( Container, 16, PutVectorTest, uint64_t, 20, 5 );
BspTest3( Container, 32, PutVectorTest, uint64_t, 20, 27 );
BspTest3( Container, 2, PutVectorTest, uint64_t, 50, 1 );
BspTest3( Container, 4, PutVectorTest, uint64_t, 50, 3 );
BspTest3( Container, 8, PutVectorTest, uint64_t, 50, 7 );
BspTest3( Container, 16, PutVectorTest, uint64_t, 50, 5 );
BspTest3( Container, 32, PutVectorTest, uint64_t, 50, 27 );

BspTest3( Container, 32, PutVectorTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, PutVectorTest, uint64_t, 100000, 27 );

BspTest3( Container, 2, PutFPVectorTest, float, 20, 1 );
BspTest3( Container, 4, PutFPVectorTest, float, 20, 3 );
BspTest3( Container, 8, PutFPVectorTest, float, 20, 7 );
BspTest3( Container, 16, PutFPVectorTest, float, 20, 5 );
BspTest3( Container, 32, PutFPVectorTest, float, 20, 27 );
BspTest3( Container, 2, PutFPVectorTest, float, 50, 1 );
BspTest3( Container, 4, PutFPVectorTest, float, 50, 3 );
BspTest3( Container, 8, PutFPVectorTest, float, 50, 7 );
BspTest3( Container, 16, PutFPVectorTest, float, 50, 5 );
BspTest3( Container, 32, PutFPVectorTest, float, 50, 27 );

BspTest3( Container, 2, PutFPVectorTest, double, 20, 1 );
BspTest3( Container, 4, PutFPVectorTest, double, 20, 3 );
BspTest3( Container, 8, PutFPVectorTest, double, 20, 7 );
BspTest3( Container, 16, PutFPVectorTest, double, 20, 5 );
BspTest3( Container, 32, PutFPVectorTest, double, 20, 27 );
BspTest3( Container, 2, PutFPVectorTest, double, 50, 1 );
BspTest3( Container, 4, PutFPVectorTest, double, 50, 3 );
BspTest3( Container, 8, PutFPVectorTest, double, 50, 7 );
BspTest3( Container, 16, PutFPVectorTest, double, 50, 5 );
BspTest3( Container, 32, PutFPVectorTest, double, 50, 27 );

BspTest3( Container, 2, PutStdArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, PutStdArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, PutStdArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, PutStdArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, PutStdArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, PutStdArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, PutStdArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, PutStdArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, PutStdArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, PutStdArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, PutStdArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, PutStdArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, PutStdArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, PutStdArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, PutStdArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, PutStdArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, PutStdArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, PutStdArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, PutStdArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, PutStdArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, PutStdArrayTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, PutStdArrayTest, uint64_t, 10000, 27 );

BspTest3( Container, 2, PutFPStdArrayTest, float, 20, 1 );
BspTest3( Container, 4, PutFPStdArrayTest, float, 20, 3 );
BspTest3( Container, 8, PutFPStdArrayTest, float, 20, 7 );
BspTest3( Container, 16, PutFPStdArrayTest, float, 20, 5 );
BspTest3( Container, 32, PutFPStdArrayTest, float, 20, 27 );
BspTest3( Container, 2, PutFPStdArrayTest, float, 50, 1 );
BspTest3( Container, 4, PutFPStdArrayTest, float, 50, 3 );
BspTest3( Container, 8, PutFPStdArrayTest, float, 50, 7 );
BspTest3( Container, 16, PutFPStdArrayTest, float, 50, 5 );
BspTest3( Container, 32, PutFPStdArrayTest, float, 50, 27 );

BspTest3( Container, 2, PutFPStdArrayTest, double, 20, 1 );
BspTest3( Container, 4, PutFPStdArrayTest, double, 20, 3 );
BspTest3( Container, 8, PutFPStdArrayTest, double, 20, 7 );
BspTest3( Container, 16, PutFPStdArrayTest, double, 20, 5 );
BspTest3( Container, 32, PutFPStdArrayTest, double, 20, 27 );
BspTest3( Container, 2, PutFPStdArrayTest, double, 50, 1 );
BspTest3( Container, 4, PutFPStdArrayTest, double, 50, 3 );
BspTest3( Container, 8, PutFPStdArrayTest, double, 50, 7 );
BspTest3( Container, 16, PutFPStdArrayTest, double, 50, 5 );
BspTest3( Container, 32, PutFPStdArrayTest, double, 50, 27 );

BspTest3( Container, 2, PutCArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, PutCArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, PutCArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, PutCArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, PutCArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, PutCArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, PutCArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, PutCArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, PutCArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, PutCArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, PutCArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, PutCArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, PutCArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, PutCArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, PutCArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, PutCArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, PutCArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, PutCArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, PutCArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, PutCArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, PutCArrayTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, PutCArrayTest, uint64_t, 10000, 27 );

BspTest3( Container, 2, PutFPCArrayTest, float, 20, 1 );
BspTest3( Container, 4, PutFPCArrayTest, float, 20, 3 );
BspTest3( Container, 8, PutFPCArrayTest, float, 20, 7 );
BspTest3( Container, 16, PutFPCArrayTest, float, 20, 5 );
BspTest3( Container, 32, PutFPCArrayTest, float, 20, 27 );
BspTest3( Container, 2, PutFPCArrayTest, float, 50, 1 );
BspTest3( Container, 4, PutFPCArrayTest, float, 50, 3 );
BspTest3( Container, 8, PutFPCArrayTest, float, 50, 7 );
BspTest3( Container, 16, PutFPCArrayTest, float, 50, 5 );
BspTest3( Container, 32, PutFPCArrayTest, float, 50, 27 );

BspTest3( Container, 2, PutFPCArrayTest, double, 20, 1 );
BspTest3( Container, 4, PutFPCArrayTest, double, 20, 3 );
BspTest3( Container, 8, PutFPCArrayTest, double, 20, 7 );
BspTest3( Container, 16, PutFPCArrayTest, double, 20, 5 );
BspTest3( Container, 32, PutFPCArrayTest, double, 20, 27 );
BspTest3( Container, 2, PutFPCArrayTest, double, 50, 1 );
BspTest3( Container, 4, PutFPCArrayTest, double, 50, 3 );
BspTest3( Container, 8, PutFPCArrayTest, double, 50, 7 );
BspTest3( Container, 16, PutFPCArrayTest, double, 50, 5 );
BspTest3( Container, 32, PutFPCArrayTest, double, 50, 27 );

#endif