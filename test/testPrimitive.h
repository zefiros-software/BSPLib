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