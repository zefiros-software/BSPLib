/**
* Copyright (c) 2016 Mick van Duijn, Koen Visscher and Paul Visscher
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

#include <array>

template< int32_t tOffset, typename tPrimitive >
void PutPaddedPrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push();

    BSPLib::Push( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Sync();

    EXPECT_EQ( result, sSource + 1 );
}

template< int32_t tOffset, typename tPrimitive >
void PutPaddedTwicePrimitiveTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    tPrimitive primitive = ( tPrimitive )s + 1;

    tPrimitive result = 0;

    BSPLib::Push();
    BSPLib::Push();

    BSPLib::Push( result );

    BSPLib::Sync();

    BSPLib::Put( sTarget, primitive, result );

    BSPLib::Pop( result );
    BSPLib::Pop();
    BSPLib::Pop();

    BSPLib::Sync();

    EXPECT_EQ( result, sSource + 1 );
}

inline void BSPAbortMessageTest()
{
    for ( uint32_t i = 0; i < 100; ++i )
    {
        EXPECT_EQ( "testing " + std::to_string( i ), BSPInternal::BspAbort( "testing " + std::to_string( i ) ).what() );
    }
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagVectorOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    std::vector< tTagPrimitive > tagContainer( tCount );
    std::vector< tTagPrimitive > receiveTag( tCount, ( tTagPrimitive )s );
    std::vector< tTagPrimitive > expectedTag( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        tagContainer[i] = ( tTagPrimitive )( s + 1 + i );
        expectedTag[i] = ( tTagPrimitive )( sReceive + 1 + i );
    }

    size_t status = 0;

    BSPLib::SetTagsize< tTagPrimitive >( tCount );

    BSPLib::Sync();

    BSPLib::SendPtrs( sSend, tagContainer, &message, ( &message ) + 1 );

    BSPLib::Sync();

    BSPLib::GetTagContainer( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< int32_t tOffset, typename tTag >
void TagPrimitiveOverloadTest()
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

    BSPLib::SendPtrs( sSend, tag, &message, ( &message ) + 1 );

    BSPLib::Sync();

    BSPLib::GetTag( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
    EXPECT_EQ( expectedTag, receiveTag );
    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagStdArrayOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    std::array< tTagPrimitive, tCount > tagContainer;
    std::array< tTagPrimitive, tCount > receiveTag;
    std::array< tTagPrimitive, tCount > expectedTag;

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        tagContainer[i] = ( tTagPrimitive )( s + 1 + i );
        expectedTag[i] = ( tTagPrimitive )( sReceive + 1 + i );
        receiveTag[i] = ( tTagPrimitive )s;
    }

    size_t status = 0;

    BSPLib::SetTagsize< tTagPrimitive >( tCount );

    BSPLib::Sync();

    BSPLib::SendPtrs( sSend, tagContainer, &message, ( &message ) + 1 );

    BSPLib::Sync();

    BSPLib::GetTagContainer( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagCArrayOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTagPrimitive tagContainer[tCount];
    tTagPrimitive receiveTag[tCount];
    tTagPrimitive expectedTag[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        tagContainer[i] = ( tTagPrimitive )( s + 1 + i );
        expectedTag[i] = ( tTagPrimitive )( sReceive + 1 + i );
        receiveTag[i] = ( tTagPrimitive )s;
    }

    size_t status = 0;

    BSPLib::SetTagsize< tTagPrimitive >( tCount );

    BSPLib::Sync();

    BSPLib::SendPtrsWithCArray( sSend, tagContainer, &message, ( &message ) + 1 );

    BSPLib::Sync();

    BSPLib::GetTagCArray( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagCArrayOverloadTest2()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTagPrimitive tagContainer[tCount];
    tTagPrimitive receiveTag[tCount];
    tTagPrimitive expectedTag[tCount];

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        tagContainer[i] = ( tTagPrimitive )( s + 1 + i );
        expectedTag[i] = ( tTagPrimitive )( sReceive + 1 + i );
        receiveTag[i] = ( tTagPrimitive )s;
    }

    size_t status = 0;

    BSPLib::SetTagsize< tTagPrimitive >( tCount );

    BSPLib::Sync();

    BSPLib::SendPtrsWithCArray( sSend, tagContainer, &message, 1 );

    BSPLib::Sync();

    BSPLib::GetTagCArray( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< int32_t tOffset >
void TagPrimitiveStringOverloadTest()
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

    BSPLib::SendPtrs( sSend, tag, &message, ( &message ) + 1 );

    BSPLib::Sync();

    BSPLib::GetTag( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );
    EXPECT_EQ( expectedTag, receiveTag );
    EXPECT_EQ( sizeof( uint32_t ), status );
}

BspTest2( Extra, 2, PutPaddedPrimitiveTest, 1, uint8_t );
BspTest2( Extra, 4, PutPaddedPrimitiveTest, 3, uint8_t );
BspTest2( Extra, 8, PutPaddedPrimitiveTest, 7, uint8_t );
BspTest2( Extra, 16, PutPaddedPrimitiveTest, 5, uint8_t );
BspTest2( Extra, 32, PutPaddedPrimitiveTest, 13, uint8_t );
BspTest2( Extra, 2, PutPaddedPrimitiveTest, 1, uint16_t );
BspTest2( Extra, 4, PutPaddedPrimitiveTest, 3, uint16_t );
BspTest2( Extra, 8, PutPaddedPrimitiveTest, 7, uint16_t );
BspTest2( Extra, 16, PutPaddedPrimitiveTest, 5, uint16_t );
BspTest2( Extra, 32, PutPaddedPrimitiveTest, 13, uint16_t );
BspTest2( Extra, 2, PutPaddedPrimitiveTest, 1, uint32_t );
BspTest2( Extra, 4, PutPaddedPrimitiveTest, 3, uint32_t );
BspTest2( Extra, 8, PutPaddedPrimitiveTest, 7, uint32_t );
BspTest2( Extra, 16, PutPaddedPrimitiveTest, 5, uint32_t );
BspTest2( Extra, 32, PutPaddedPrimitiveTest, 13, uint32_t );
BspTest2( Extra, 2, PutPaddedPrimitiveTest, 1, uint64_t );
BspTest2( Extra, 4, PutPaddedPrimitiveTest, 3, uint64_t );
BspTest2( Extra, 8, PutPaddedPrimitiveTest, 7, uint64_t );
BspTest2( Extra, 16, PutPaddedPrimitiveTest, 5, uint64_t );
BspTest2( Extra, 32, PutPaddedPrimitiveTest, 13, uint64_t );

BspTest2( Extra, 2, PutPaddedTwicePrimitiveTest, 1, uint8_t );
BspTest2( Extra, 4, PutPaddedTwicePrimitiveTest, 3, uint8_t );
BspTest2( Extra, 8, PutPaddedTwicePrimitiveTest, 7, uint8_t );
BspTest2( Extra, 16, PutPaddedTwicePrimitiveTest, 5, uint8_t );
BspTest2( Extra, 32, PutPaddedTwicePrimitiveTest, 13, uint8_t );
BspTest2( Extra, 2, PutPaddedTwicePrimitiveTest, 1, uint16_t );
BspTest2( Extra, 4, PutPaddedTwicePrimitiveTest, 3, uint16_t );
BspTest2( Extra, 8, PutPaddedTwicePrimitiveTest, 7, uint16_t );
BspTest2( Extra, 16, PutPaddedTwicePrimitiveTest, 5, uint16_t );
BspTest2( Extra, 32, PutPaddedTwicePrimitiveTest, 13, uint16_t );
BspTest2( Extra, 2, PutPaddedTwicePrimitiveTest, 1, uint32_t );
BspTest2( Extra, 4, PutPaddedTwicePrimitiveTest, 3, uint32_t );
BspTest2( Extra, 8, PutPaddedTwicePrimitiveTest, 7, uint32_t );
BspTest2( Extra, 16, PutPaddedTwicePrimitiveTest, 5, uint32_t );
BspTest2( Extra, 32, PutPaddedTwicePrimitiveTest, 13, uint32_t );
BspTest2( Extra, 2, PutPaddedTwicePrimitiveTest, 1, uint64_t );
BspTest2( Extra, 4, PutPaddedTwicePrimitiveTest, 3, uint64_t );
BspTest2( Extra, 8, PutPaddedTwicePrimitiveTest, 7, uint64_t );
BspTest2( Extra, 16, PutPaddedTwicePrimitiveTest, 5, uint64_t );
BspTest2( Extra, 32, PutPaddedTwicePrimitiveTest, 13, uint64_t );

BspTest( Extra, 32, BSPAbortMessageTest );

BspTest3( Extra, 8, TagVectorOverloadTest, uint32_t, 23, 5 );
BspTest3( Extra, 8, TagStdArrayOverloadTest, uint32_t, 23, 5 );
BspTest3( Extra, 8, TagCArrayOverloadTest, uint32_t, 23, 5 );
BspTest3( Extra, 8, TagCArrayOverloadTest2, uint32_t, 23, 5 );
BspTest2( Extra, 8, TagPrimitiveOverloadTest, 5, uint32_t );
BspTest1( Extra, 8, TagPrimitiveStringOverloadTest, 5 );