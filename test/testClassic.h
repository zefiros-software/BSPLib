#pragma once
#ifndef __TESTCLASSIC_H__
#define __TESTCLASSIC_H__

#include "helper.h"

inline void EmptyTest()
{
}

template< uint32_t tProcs >
void NProcsTest()
{
    uint32_t nProcs = BSPLib::NProcs();

    EXPECT_EQ( tProcs, nProcs );
}

template< uint32_t tSyncs >
void SyncTest()
{
    for ( uint32_t i = 0; i < tSyncs; ++i )
    {
        BSPLib::Sync();
    }
}

template< uint32_t tPopRegs >
void PushPopTest()
{
    uint32_t reg;

    for ( uint32_t i = 0; i < tPopRegs; ++i )
    {
        BSPLib::Classic::PushReg( &reg, sizeof( uint32_t ) );
        BSPLib::Sync();

        BSPLib::Classic::PopReg( &reg );
    }
}

template< uint32_t tPuts, int32_t tOffset >
void PutTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();
    uint32_t to = ( s + tOffset + nProc ) % nProc;

    // Try to mess with the registers (should not go wrong)
    PushPopTest< tPuts >();

    uint32_t expected = ( ( s - tOffset + nProc ) % nProc ) + 1;

    uint32_t num = s + 1;
    uint32_t receive = 0;

    BSPLib::Classic::PushReg( &receive, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tPuts; ++i )
    {
        BSPLib::Classic::Put( to, &num, &receive, 0, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }

    BSPLib::Classic::PopReg( &receive );
}

template< uint32_t tGets, int32_t tOffset >
void GetTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();
    uint32_t from = ( s + tOffset + nProc ) % nProc;

    uint32_t expected = from + 1;

    uint32_t num = s + 1;
    uint32_t receive = 0;

    BSPLib::Classic::PushReg( &num, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tGets; ++i )
    {
        BSPLib::Classic::Get( from, &num, 0, &receive, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }

    BSPLib::Classic::PopReg( &num );
}

template< uint32_t tPutGets, int32_t tOffset >
void MixedPutGetTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sRetrieve = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t expectedReceived = sRetrieve + 1;
    uint32_t expectedRetrieved = sRetrieve + 1;

    uint32_t num = s + 1;
    uint32_t retrieved = 0;
    uint32_t received = 0;


    BSPLib::Classic::PushReg( &num, sizeof( uint32_t ) );
    BSPLib::Classic::PushReg( &received, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tPutGets; ++i )
    {
        BSPLib::Classic::Get( sRetrieve, &num, 0, &retrieved, sizeof( uint32_t ) );
        BSPLib::Classic::Put( sReceive, &num, &received, 0, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expectedReceived, received );
        EXPECT_EQ( expectedRetrieved, retrieved );

        received = 0;
        retrieved = 0;
    }

    BSPLib::Classic::PopReg( &received );
    BSPLib::Classic::PopReg( &num );
}

template< uint32_t tSends, int32_t tOffset >
void SendTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    for ( uint32_t i = 0; i < nProc; ++i )
    {
        BSPLib::Classic::Send( sSend, NULL, &message, sizeof( uint32_t ) );

        BSPLib::Sync();

        BSPLib::Classic::Move( &mailbox, sizeof( uint32_t ) );

        EXPECT_EQ( expectedMail, mailbox );
        mailbox = s;
    }
}

template< uint32_t tSends, int32_t tOffset, typename tTag >
void TagTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTag tag = static_cast< tTag >( ( s - 1 + nProc ) % nProc );

    tTag receiveTag = static_cast< tTag >( s );
    tTag expectedTag = static_cast< tTag >( ( sReceive - 1 + nProc ) % nProc );

    size_t tagSize = sizeof( tTag );
    size_t status = 0;

    BSPLib::Classic::SetTagSize( &tagSize );

    for ( uint32_t i = 0; i < tSends; ++i )
    {
        BSPLib::Sync();

        BSPLib::Classic::Send( sSend, &tag, &message, sizeof( uint32_t ) );

        BSPLib::Sync();

        BSPLib::Classic::GetTag( &status, &receiveTag );

        BSPLib::Classic::Move( &mailbox, sizeof( uint32_t ) );

        EXPECT_EQ( expectedMail, mailbox );
        EXPECT_EQ( expectedTag, receiveTag );
        EXPECT_EQ( sizeof( uint32_t ), status );
    }
}

inline void TimerTest()
{
    BSPLib::Sync();

    double start = BSPLib::Time();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10000 ) );
    double end = BSPLib::Time();

    EXPECT_NEAR( end - start, 10.0, 0.1 );
}

inline void AbortTest()
{
    BSPLib::Sync();
    BSPLib::Classic::Abort( "" );
}

TEST( P( Classic ), AbortTest )
{
    try
    {
        BspExecute( AbortTest, 32 );
    }
    catch ( std::exception &e )
    {
        ASSERT_STREQ( "Aborted", e.what() );
    }
}

BspTest( Classic, 1, EmptyTest );
BspTest( Classic, 2, EmptyTest );
BspTest( Classic, 4, EmptyTest );
BspTest( Classic, 8, EmptyTest );
BspTest( Classic, 16, EmptyTest );
BspTest( Classic, 32, EmptyTest );

BspTest1( Classic, 1, NProcsTest, 1 );
BspTest1( Classic, 2, NProcsTest, 2 );
BspTest1( Classic, 4, NProcsTest, 4 );
BspTest1( Classic, 8, NProcsTest, 8 );
BspTest1( Classic, 16, NProcsTest, 16 );
BspTest1( Classic, 32, NProcsTest, 32 );

BspTest1( Classic, 1, SyncTest, 1 );
BspTest1( Classic, 2, SyncTest, 1 );
BspTest1( Classic, 4, SyncTest, 1 );
BspTest1( Classic, 8, SyncTest, 1 );
BspTest1( Classic, 1, SyncTest, 2 );
BspTest1( Classic, 2, SyncTest, 2 );
BspTest1( Classic, 4, SyncTest, 2 );
BspTest1( Classic, 8, SyncTest, 2 );
BspTest1( Classic, 1, SyncTest, 4 );
BspTest1( Classic, 2, SyncTest, 4 );
BspTest1( Classic, 4, SyncTest, 4 );
BspTest1( Classic, 8, SyncTest, 4 );
BspTest1( Classic, 1, SyncTest, 8 );
BspTest1( Classic, 2, SyncTest, 8 );
BspTest1( Classic, 4, SyncTest, 8 );
BspTest1( Classic, 8, SyncTest, 8 );
BspTest1( Classic, 1, SyncTest, 16 );
BspTest1( Classic, 2, SyncTest, 16 );
BspTest1( Classic, 4, SyncTest, 16 );
BspTest1( Classic, 8, SyncTest, 16 );
BspTest1( Classic, 32, SyncTest, 128 );

BspTest1( Classic, 32, PushPopTest, 128 );

BspTest( Classic, 32, TimerTest );

BspTest2( Classic, 2, PutTest, 2, 1 );
BspTest2( Classic, 4, PutTest, 2, 1 );
BspTest2( Classic, 8, PutTest, 2, 1 );
BspTest2( Classic, 16, PutTest, 2, 1 );
BspTest2( Classic, 32, PutTest, 2, 1 );
BspTest2( Classic, 8, PutTest, 4, 3 );
BspTest2( Classic, 16, PutTest, 4, 3 );
BspTest2( Classic, 32, PutTest, 4, 3 );
BspTest2( Classic, 8, PutTest, 7, 3 );
BspTest2( Classic, 16, PutTest, 7, 3 );
BspTest2( Classic, 32, PutTest, 7, 3 );
BspTest2( Classic, 32, PutTest, 100, 41 );

BspTest2( Classic, 2, GetTest, 2, 1 );
BspTest2( Classic, 4, GetTest, 2, 1 );
BspTest2( Classic, 8, GetTest, 2, 1 );
BspTest2( Classic, 16, GetTest, 2, 1 );
BspTest2( Classic, 32, GetTest, 2, 1 );
BspTest2( Classic, 8, GetTest, 4, 3 );
BspTest2( Classic, 16, GetTest, 4, 3 );
BspTest2( Classic, 32, GetTest, 4, 3 );
BspTest2( Classic, 8, GetTest, 7, 3 );
BspTest2( Classic, 16, GetTest, 7, 3 );
BspTest2( Classic, 32, GetTest, 7, 3 );
BspTest2( Classic, 32, GetTest, 100, 41 );

BspTest2( Classic, 2, MixedPutGetTest, 2, 1 );
BspTest2( Classic, 4, MixedPutGetTest, 2, 1 );
BspTest2( Classic, 8, MixedPutGetTest, 2, 1 );
BspTest2( Classic, 16, MixedPutGetTest, 2, 1 );
BspTest2( Classic, 32, MixedPutGetTest, 2, 1 );
BspTest2( Classic, 8, MixedPutGetTest, 4, 3 );
BspTest2( Classic, 16, MixedPutGetTest, 4, 3 );
BspTest2( Classic, 32, MixedPutGetTest, 4, 3 );
BspTest2( Classic, 8, MixedPutGetTest, 7, 3 );
BspTest2( Classic, 16, MixedPutGetTest, 7, 3 );
BspTest2( Classic, 32, MixedPutGetTest, 7, 3 );
BspTest2( Classic, 32, MixedPutGetTest, 100, 41 );

BspTest2( Classic, 2, SendTest, 2, 1 );
BspTest2( Classic, 4, SendTest, 2, 1 );
BspTest2( Classic, 8, SendTest, 2, 1 );
BspTest2( Classic, 16, SendTest, 2, 1 );
BspTest2( Classic, 32, SendTest, 2, 1 );
BspTest2( Classic, 8, SendTest, 4, 3 );
BspTest2( Classic, 16, SendTest, 4, 3 );
BspTest2( Classic, 32, SendTest, 4, 3 );
BspTest2( Classic, 8, SendTest, 7, 3 );
BspTest2( Classic, 16, SendTest, 7, 3 );
BspTest2( Classic, 32, SendTest, 7, 3 );
BspTest2( Classic, 32, SendTest, 100, 41 );

BspTest3( Classic, 2, TagTest, 2, 1, uint8_t );
BspTest3( Classic, 4, TagTest, 2, 1, uint8_t );
BspTest3( Classic, 8, TagTest, 2, 1, uint8_t );
BspTest3( Classic, 16, TagTest, 2, 1, uint8_t );
BspTest3( Classic, 32, TagTest, 2, 1, uint8_t );
BspTest3( Classic, 8, TagTest, 4, 3, uint8_t );
BspTest3( Classic, 16, TagTest, 4, 3, uint8_t );
BspTest3( Classic, 32, TagTest, 4, 3, uint8_t );
BspTest3( Classic, 8, TagTest, 7, 3, uint8_t );
BspTest3( Classic, 16, TagTest, 7, 3, uint8_t );
BspTest3( Classic, 32, TagTest, 7, 3, uint8_t );
BspTest3( Classic, 32, TagTest, 100, 41, uint8_t );

BspTest3( Classic, 2, TagTest, 2, 1, uint16_t );
BspTest3( Classic, 4, TagTest, 2, 1, uint16_t );
BspTest3( Classic, 8, TagTest, 2, 1, uint16_t );
BspTest3( Classic, 16, TagTest, 2, 1, uint16_t );
BspTest3( Classic, 32, TagTest, 2, 1, uint16_t );
BspTest3( Classic, 8, TagTest, 4, 3, uint16_t );
BspTest3( Classic, 16, TagTest, 4, 3, uint16_t );
BspTest3( Classic, 32, TagTest, 4, 3, uint16_t );
BspTest3( Classic, 8, TagTest, 7, 3, uint16_t );
BspTest3( Classic, 16, TagTest, 7, 3, uint16_t );
BspTest3( Classic, 32, TagTest, 7, 3, uint16_t );
BspTest3( Classic, 32, TagTest, 100, 41, uint16_t );

BspTest3( Classic, 2, TagTest, 2, 1, uint32_t );
BspTest3( Classic, 4, TagTest, 2, 1, uint32_t );
BspTest3( Classic, 8, TagTest, 2, 1, uint32_t );
BspTest3( Classic, 16, TagTest, 2, 1, uint32_t );
BspTest3( Classic, 32, TagTest, 2, 1, uint32_t );
BspTest3( Classic, 8, TagTest, 4, 3, uint32_t );
BspTest3( Classic, 16, TagTest, 4, 3, uint32_t );
BspTest3( Classic, 32, TagTest, 4, 3, uint32_t );
BspTest3( Classic, 8, TagTest, 7, 3, uint32_t );
BspTest3( Classic, 16, TagTest, 7, 3, uint32_t );
BspTest3( Classic, 32, TagTest, 7, 3, uint32_t );
BspTest3( Classic, 32, TagTest, 100, 41, uint32_t );

BspTest3( Classic, 2, TagTest, 2, 1, uint64_t );
BspTest3( Classic, 4, TagTest, 2, 1, uint64_t );
BspTest3( Classic, 8, TagTest, 2, 1, uint64_t );
BspTest3( Classic, 16, TagTest, 2, 1, uint64_t );
BspTest3( Classic, 32, TagTest, 2, 1, uint64_t );
BspTest3( Classic, 8, TagTest, 4, 3, uint64_t );
BspTest3( Classic, 16, TagTest, 4, 3, uint64_t );
BspTest3( Classic, 32, TagTest, 4, 3, uint64_t );
BspTest3( Classic, 8, TagTest, 7, 3, uint64_t );
BspTest3( Classic, 16, TagTest, 7, 3, uint64_t );
BspTest3( Classic, 32, TagTest, 7, 3, uint64_t );
BspTest3( Classic, 32, TagTest, 100, 41, uint64_t );

#endif