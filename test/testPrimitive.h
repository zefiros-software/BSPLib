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

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Pop( result );

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

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Sync();

    ASSERT_DOUBLE_EQ( sSource + 1, result );
}

template< int32_t tOffset >
void PutPrimitiveStringTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::string primitive = std::to_string( s + 1 );

    std::string result = std::to_string( 0 );

    size_t resultSize = result.size();

    BSPLib::Push( resultSize );

    BSPLib::Sync();

    resultSize = primitive.size();
    BSPLib::Put( sTarget, resultSize );

    BSPLib::Sync();

    result.resize( resultSize );

    BSPLib::Push( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Sync();

    std::string expected = std::to_string( sSource + 1 );

    EXPECT_EQ( expected, result );
}

template< int32_t tOffset, typename tPrimitive >
void PutSamePrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive );

    BSPLib::Sync();

    ASSERT_EQ( primitive, sSource + 1 );
}

template< int32_t tOffset, typename tPrimitive >
void PutSamePrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive );

    BSPLib::Sync();

    ASSERT_DOUBLE_EQ( sSource + 1, primitive );
}

template< int32_t tOffset, typename tPrimitive >
void GetPrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Get( sSource, primitive, result );

    BSPLib::Pop( primitive );

    BSPLib::Sync();

    ASSERT_EQ( result, sSource + 1 );
}

template< int32_t tOffset, typename tPrimitive >
void GetPrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Get( sSource, primitive, result );

    BSPLib::Sync();

    ASSERT_DOUBLE_EQ( sSource + 1, result );
}

template< int32_t tOffset >
void GetPrimitiveStringTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::string primitive = std::to_string( s + 1 );

    std::string result = std::to_string( 0 );

    size_t primitiveSize = primitive.size();

    BSPLib::Push( primitiveSize );
    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Get( sSource, primitiveSize );

    BSPLib::Sync();

    result.resize( primitiveSize );

    BSPLib::Get( sSource, primitive, result );
    BSPLib::Pop( primitive );
    BSPLib::Pop( primitiveSize );

    BSPLib::Sync();

    std::string expected = std::to_string( sSource + 1 );

    EXPECT_EQ( expected, result );
}

template< int32_t tOffset, typename tPrimitive >
void GetSamePrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Get( sSource, primitive );

    BSPLib::Sync();

    ASSERT_EQ( primitive, sSource + 1 );
}

template< int32_t tOffset, typename tPrimitive >
void GetSamePrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    BSPLib::Push( primitive );

    BSPLib::Sync();

    BSPLib::Get( sSource, primitive );

    BSPLib::Sync();

    ASSERT_DOUBLE_EQ( sSource + 1, primitive );
}

template< int32_t tOffset, typename tPrimitive >
void SendPrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message = ( tPrimitive )s + 1;
    tPrimitive mailbox = ( tPrimitive )s;

    tPrimitive expectedMail = ( tPrimitive )sReceive + 1;

    BSPLib::Send( sSend, message );

    BSPLib::Sync();

    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
}

template< int32_t tOffset, typename tPrimitive >
void SendPrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message = ( tPrimitive )s + 1;
    tPrimitive mailbox = ( tPrimitive )s;

    tPrimitive expectedMail = ( tPrimitive )sReceive + 1;

    BSPLib::Send( sSend, message );

    BSPLib::Sync();

    BSPLib::Move( mailbox );

    EXPECT_DOUBLE_EQ( expectedMail, mailbox );
}

template< int32_t tOffset >
void SendPrimitiveStringTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::string message = std::to_string( s + 1 );
    std::string mailbox = std::to_string( s );

    std::string expectedMail = std::to_string( sReceive + 1 );
    size_t size = message.size();

    BSPLib::SetTagsize< size_t >();

    BSPLib::Sync();

    BSPLib::Send( sSend, size, message );

    BSPLib::Sync();

    size_t status;

    BSPLib::GetTag( status, size );

    mailbox.resize( size );

    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
}

template< int32_t tOffset, typename tTag >
void TagPrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTag tag = static_cast<tTag>( ( s - 1 + nProc ) % nProc );

    tTag receiveTag = static_cast<tTag>( s );
    tTag expectedTag = static_cast<tTag>( ( sReceive - 1 + nProc ) % nProc );

    size_t status = 0;

    BSPLib::SetTagsize< tTag >();

    BSPLib::Sync();

    BSPLib::Send( sSend, tag, message );

    BSPLib::Sync();

    BSPLib::GetTag( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
    EXPECT_EQ( expectedTag, receiveTag );
    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< int32_t tOffset, typename tTag >
void TagPrimitiveFPTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTag tag = static_cast<tTag>( ( s - 1 + nProc ) % nProc );

    tTag receiveTag = static_cast<tTag>( s );
    tTag expectedTag = static_cast<tTag>( ( sReceive - 1 + nProc ) % nProc );

    size_t status = 0;

    BSPLib::SetTagsize< tTag >();

    BSPLib::Sync();

    BSPLib::Send( sSend, tag, message );

    BSPLib::Sync();

    BSPLib::GetTag( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
    EXPECT_DOUBLE_EQ( expectedTag, receiveTag );
    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< int32_t tOffset >
void TagPrimitiveStringTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    std::stringstream ss;

    ss << std::setfill( '0' ) << std::setw( 32 ) << ( ( s - 1 + nProc ) % nProc );
    std::string tag( ss.str() );
    ss.str( "" );

    ss << std::setfill( '0' ) << std::setw( 32 ) << s;
    std::string receiveTag( ss.str() );
    ss.str( "" );

    ss << std::setfill( '0' ) << std::setw( 32 ) << ( ( sReceive - 1 + nProc ) % nProc );
    std::string expectedTag( ss.str() );
    ss.str( "" );

    size_t status = 0;

    BSPLib::SetTagsize< char >( 32 );

    BSPLib::Sync();

    BSPLib::Send( sSend, tag, message );

    BSPLib::Sync();

    BSPLib::GetTag( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
    EXPECT_EQ( expectedTag, receiveTag );
    EXPECT_EQ( sizeof( uint32_t ), status );
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

BspTest1( Primitive, 2, PutPrimitiveStringTest, 1 );
BspTest1( Primitive, 4, PutPrimitiveStringTest, 3 );
BspTest1( Primitive, 8, PutPrimitiveStringTest, 7 );
BspTest1( Primitive, 16, PutPrimitiveStringTest, 5 );
BspTest1( Primitive, 32, PutPrimitiveStringTest, 13 );

BspTest2( Primitive, 2, PutSamePrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, PutSamePrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, PutSamePrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, PutSamePrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, PutSamePrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, PutSamePrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, PutSamePrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, PutSamePrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, PutSamePrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, PutSamePrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, PutSamePrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, PutSamePrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, PutSamePrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, PutSamePrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, PutSamePrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, PutSamePrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, PutSamePrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, PutSamePrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, PutSamePrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, PutSamePrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, PutSamePrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, PutSamePrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, PutSamePrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, PutSamePrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, PutSamePrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, PutSamePrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, PutSamePrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, PutSamePrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, PutSamePrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, PutSamePrimitiveFPTest, 13, double );

BspTest2( Primitive, 2, GetPrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, GetPrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, GetPrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, GetPrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, GetPrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, GetPrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, GetPrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, GetPrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, GetPrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, GetPrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, GetPrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, GetPrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, GetPrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, GetPrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, GetPrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, GetPrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, GetPrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, GetPrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, GetPrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, GetPrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, GetPrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, GetPrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, GetPrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, GetPrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, GetPrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, GetPrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, GetPrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, GetPrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, GetPrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, GetPrimitiveFPTest, 13, double );

BspTest1( Primitive, 2, GetPrimitiveStringTest, 1 );
BspTest1( Primitive, 4, GetPrimitiveStringTest, 3 );
BspTest1( Primitive, 8, GetPrimitiveStringTest, 7 );
BspTest1( Primitive, 16, GetPrimitiveStringTest, 5 );
BspTest1( Primitive, 32, GetPrimitiveStringTest, 13 );

BspTest2( Primitive, 2, GetSamePrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, GetSamePrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, GetSamePrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, GetSamePrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, GetSamePrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, GetSamePrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, GetSamePrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, GetSamePrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, GetSamePrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, GetSamePrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, GetSamePrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, GetSamePrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, GetSamePrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, GetSamePrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, GetSamePrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, GetSamePrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, GetSamePrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, GetSamePrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, GetSamePrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, GetSamePrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, GetSamePrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, GetSamePrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, GetSamePrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, GetSamePrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, GetSamePrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, GetSamePrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, GetSamePrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, GetSamePrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, GetSamePrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, GetSamePrimitiveFPTest, 13, double );



BspTest2( Primitive, 2, SendPrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, SendPrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, SendPrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, SendPrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, SendPrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, SendPrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, SendPrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, SendPrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, SendPrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, SendPrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, SendPrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, SendPrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, SendPrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, SendPrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, SendPrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, SendPrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, SendPrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, SendPrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, SendPrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, SendPrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, SendPrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, SendPrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, SendPrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, SendPrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, SendPrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, SendPrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, SendPrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, SendPrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, SendPrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, SendPrimitiveFPTest, 13, double );

BspTest1( Primitive, 2, SendPrimitiveStringTest, 1 );
BspTest1( Primitive, 4, SendPrimitiveStringTest, 3 );
BspTest1( Primitive, 8, SendPrimitiveStringTest, 7 );
BspTest1( Primitive, 16, SendPrimitiveStringTest, 5 );
BspTest1( Primitive, 32, SendPrimitiveStringTest, 13 );



BspTest2( Primitive, 2, TagPrimitiveTest, 1, uint8_t );
BspTest2( Primitive, 4, TagPrimitiveTest, 3, uint8_t );
BspTest2( Primitive, 8, TagPrimitiveTest, 7, uint8_t );
BspTest2( Primitive, 16, TagPrimitiveTest, 5, uint8_t );
BspTest2( Primitive, 32, TagPrimitiveTest, 13, uint8_t );
BspTest2( Primitive, 2, TagPrimitiveTest, 1, uint16_t );
BspTest2( Primitive, 4, TagPrimitiveTest, 3, uint16_t );
BspTest2( Primitive, 8, TagPrimitiveTest, 7, uint16_t );
BspTest2( Primitive, 16, TagPrimitiveTest, 5, uint16_t );
BspTest2( Primitive, 32, TagPrimitiveTest, 13, uint16_t );
BspTest2( Primitive, 2, TagPrimitiveTest, 1, uint32_t );
BspTest2( Primitive, 4, TagPrimitiveTest, 3, uint32_t );
BspTest2( Primitive, 8, TagPrimitiveTest, 7, uint32_t );
BspTest2( Primitive, 16, TagPrimitiveTest, 5, uint32_t );
BspTest2( Primitive, 32, TagPrimitiveTest, 13, uint32_t );
BspTest2( Primitive, 2, TagPrimitiveTest, 1, uint64_t );
BspTest2( Primitive, 4, TagPrimitiveTest, 3, uint64_t );
BspTest2( Primitive, 8, TagPrimitiveTest, 7, uint64_t );
BspTest2( Primitive, 16, TagPrimitiveTest, 5, uint64_t );
BspTest2( Primitive, 32, TagPrimitiveTest, 13, uint64_t );

BspTest2( Primitive, 2, TagPrimitiveFPTest, 1, float );
BspTest2( Primitive, 4, TagPrimitiveFPTest, 3, float );
BspTest2( Primitive, 8, TagPrimitiveFPTest, 7, float );
BspTest2( Primitive, 16, TagPrimitiveFPTest, 5, float );
BspTest2( Primitive, 32, TagPrimitiveFPTest, 13, float );
BspTest2( Primitive, 2, TagPrimitiveFPTest, 1, double );
BspTest2( Primitive, 4, TagPrimitiveFPTest, 3, double );
BspTest2( Primitive, 8, TagPrimitiveFPTest, 7, double );
BspTest2( Primitive, 16, TagPrimitiveFPTest, 5, double );
BspTest2( Primitive, 32, TagPrimitiveFPTest, 13, double );

BspTest1( Primitive, 2, TagPrimitiveStringTest, 1 );
BspTest1( Primitive, 4, TagPrimitiveStringTest, 3 );
BspTest1( Primitive, 8, TagPrimitiveStringTest, 7 );
BspTest1( Primitive, 16, TagPrimitiveStringTest, 5 );
BspTest1( Primitive, 32, TagPrimitiveStringTest, 13 );

#endif