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
#ifndef __TESTPRIMITIVE_H__
#define __TESTPRIMITIVE_H__

#include "helper.h"

template< int32_t tOffset, typename tPrimitive >
void PutPrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive;

    primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::PushReg( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Sync();

    ASSERT_EQ( result, sSource + 1 );
}

template< int32_t tOffset, typename tPrimitive >
void PutPrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive;

    primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::PushReg( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Sync();

    ASSERT_DOUBLE_EQ( sSource + 1, result );
}

BspTest2( Primitive, 2, PutPrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, PutPrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, PutPrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, PutPrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, PutPrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, PutPrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, PutPrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, PutPrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, PutPrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, PutPrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, PutPrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, PutPrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, PutPrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, PutPrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, PutPrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, PutPrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, PutPrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, PutPrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, PutPrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, PutPrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, PutPrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, PutPrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, PutPrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, PutPrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, PutPrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, PutPrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, PutPrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, PutPrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, PutPrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, PutPrimitiveFPTest, 13, double );

#endif