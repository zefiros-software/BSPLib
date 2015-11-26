#pragma once
#ifndef __TESTCLASSIC_H__
#define __TESTCLASSIC_H__

#include "helper.h"

inline void EmptyTest()
{
}

template< uint32_t tSyncs >
void SyncTest()
{
    for ( uint32_t i = 0; i < tSyncs; ++i )
    {
        BSPLib::Sync();
    }
}

template< uint32_t tProc, uint32_t tPuts, int32_t tOffset >
void PutTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t to = ( s + tOffset + tProc ) % tProc;

    uint32_t expected = ( ( s - tOffset + tProc ) % tProc ) + 1;

    uint32_t num = s + 1;
    uint32_t receive = 0;

    BSPLib::Classic::PushReg( &num, sizeof( uint32_t ) );
    BSPLib::Classic::PushReg( &receive, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tPuts; ++i )
    {
        BSPLib::Classic::Put( to, &num, &receive, 0, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }
}

template< uint32_t tProc, uint32_t tGets, int32_t tOffset >
void GetTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t from = ( s + tOffset + tProc ) % tProc;

    uint32_t expected = from + 1;

    uint32_t num = s + 1;
    uint32_t receive = 0;

    BSPLib::Classic::PushReg( &num, sizeof( uint32_t ) );
    BSPLib::Classic::PushReg( &receive, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tGets; ++i )
    {
        BSPLib::Classic::Get( from, &num, 0, &receive, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }
}

template< uint32_t tProc, uint32_t tPutGets, int32_t tOffset >
void MixedPutGetTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t sRetrieve = ( s + tOffset + tProc ) % tProc;
    uint32_t sReceive = ( s - tOffset + tProc ) % tProc;

    uint32_t expectedReceived = sRetrieve + 1;
    uint32_t expectedRetrieved = sRetrieve + 1;

    uint32_t num = s + 1;
    uint32_t retrieved = 0;
    uint32_t received = 0;


    BSPLib::Classic::PushReg( &num, sizeof( uint32_t ) );
    BSPLib::Classic::PushReg( &received, sizeof( uint32_t ) );
    BSPLib::Classic::PushReg( &retrieved, sizeof( uint32_t ) );

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
}

template< uint32_t tProc, uint32_t tSends, int32_t tOffset >
void SendTest()
{
    uint32_t s = BSPLib::ProcId();

    uint32_t sSend = ( s + tOffset + tProc ) % tProc;
    uint32_t sReceive = ( s - tOffset + tProc ) % tProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    for ( uint32_t i = 0; i < tProc; ++i )
    {
        BSPLib::Classic::Send( sSend, NULL, &message, sizeof( uint32_t ) );

        BSPLib::Sync();

        BSPLib::Classic::Move( &mailbox, sizeof( uint32_t ) );

        EXPECT_EQ( expectedMail, mailbox );
        mailbox = s;
    }
}

template< uint32_t tProc, uint32_t tSends, int32_t tOffset, typename tTag >
void TagTest()
{
    uint32_t s = BSPLib::ProcId();

    uint32_t sSend = ( s + tOffset + tProc ) % tProc;
    uint32_t sReceive = ( s - tOffset + tProc ) % tProc;

    uint32_t message = s + 1;
    uint32_t mailbox = s;

    uint32_t expectedMail = sReceive + 1;

    tTag tag = static_cast< tTag >( ( s - 1 + tProc ) % tProc );

    tTag receiveTag = static_cast< tTag >( s );
    tTag expectedTag = static_cast< tTag >( ( sReceive - 1 + tProc ) % tProc );

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

BspTest( Classic, 1, EmptyTest );
BspTest( Classic, 2, EmptyTest );
BspTest( Classic, 4, EmptyTest );
BspTest( Classic, 8, EmptyTest );
BspTest( Classic, 16, EmptyTest );
BspTest( Classic, 32, EmptyTest );

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

BspTest3( Classic, 2, PutTest, 2, 2, 1 );
BspTest3( Classic, 4, PutTest, 4, 2, 1 );
BspTest3( Classic, 8, PutTest, 8, 2, 1 );
BspTest3( Classic, 16, PutTest, 16, 2, 1 );
BspTest3( Classic, 32, PutTest, 32, 2, 1 );
BspTest3( Classic, 8, PutTest, 8, 4, 3 );
BspTest3( Classic, 16, PutTest, 16, 4, 3 );
BspTest3( Classic, 32, PutTest, 32, 4, 3 );
BspTest3( Classic, 8, PutTest, 8, 7, 3 );
BspTest3( Classic, 16, PutTest, 16, 7, 3 );
BspTest3( Classic, 32, PutTest, 32, 7, 3 );
BspTest3( Classic, 32, PutTest, 32, 100, 41 );

BspTest3( Classic, 2, GetTest, 2, 2, 1 );
BspTest3( Classic, 4, GetTest, 4, 2, 1 );
BspTest3( Classic, 8, GetTest, 8, 2, 1 );
BspTest3( Classic, 16, GetTest, 16, 2, 1 );
BspTest3( Classic, 32, GetTest, 32, 2, 1 );
BspTest3( Classic, 8, GetTest, 8, 4, 3 );
BspTest3( Classic, 16, GetTest, 16, 4, 3 );
BspTest3( Classic, 32, GetTest, 32, 4, 3 );
BspTest3( Classic, 8, GetTest, 8, 7, 3 );
BspTest3( Classic, 16, GetTest, 16, 7, 3 );
BspTest3( Classic, 32, GetTest, 32, 7, 3 );
BspTest3( Classic, 32, GetTest, 32, 100, 41 );

BspTest3( Classic, 2, MixedPutGetTest, 2, 2, 1 );
BspTest3( Classic, 4, MixedPutGetTest, 4, 2, 1 );
BspTest3( Classic, 8, MixedPutGetTest, 8, 2, 1 );
BspTest3( Classic, 16, MixedPutGetTest, 16, 2, 1 );
BspTest3( Classic, 32, MixedPutGetTest, 32, 2, 1 );
BspTest3( Classic, 8, MixedPutGetTest, 8, 4, 3 );
BspTest3( Classic, 16, MixedPutGetTest, 16, 4, 3 );
BspTest3( Classic, 32, MixedPutGetTest, 32, 4, 3 );
BspTest3( Classic, 8, MixedPutGetTest, 8, 7, 3 );
BspTest3( Classic, 16, MixedPutGetTest, 16, 7, 3 );
BspTest3( Classic, 32, MixedPutGetTest, 32, 7, 3 );
BspTest3( Classic, 32, MixedPutGetTest, 32, 100, 41 );

BspTest3( Classic, 2, SendTest, 2, 2, 1 );
BspTest3( Classic, 4, SendTest, 4, 2, 1 );
BspTest3( Classic, 8, SendTest, 8, 2, 1 );
BspTest3( Classic, 16, SendTest, 16, 2, 1 );
BspTest3( Classic, 32, SendTest, 32, 2, 1 );
BspTest3( Classic, 8, SendTest, 8, 4, 3 );
BspTest3( Classic, 16, SendTest, 16, 4, 3 );
BspTest3( Classic, 32, SendTest, 32, 4, 3 );
BspTest3( Classic, 8, SendTest, 8, 7, 3 );
BspTest3( Classic, 16, SendTest, 16, 7, 3 );
BspTest3( Classic, 32, SendTest, 32, 7, 3 );
BspTest3( Classic, 32, SendTest, 32, 100, 41 );

BspTest4( Classic, 2, TagTest, 2, 2, 1, uint8_t );
BspTest4( Classic, 4, TagTest, 4, 2, 1, uint8_t );
BspTest4( Classic, 8, TagTest, 8, 2, 1, uint8_t );
BspTest4( Classic, 16, TagTest, 16, 2, 1, uint8_t );
BspTest4( Classic, 32, TagTest, 32, 2, 1, uint8_t );
BspTest4( Classic, 8, TagTest, 8, 4, 3, uint8_t );
BspTest4( Classic, 16, TagTest, 16, 4, 3, uint8_t );
BspTest4( Classic, 32, TagTest, 32, 4, 3, uint8_t );
BspTest4( Classic, 8, TagTest, 8, 7, 3, uint8_t );
BspTest4( Classic, 16, TagTest, 16, 7, 3, uint8_t );
BspTest4( Classic, 32, TagTest, 32, 7, 3, uint8_t );
BspTest4( Classic, 32, TagTest, 32, 100, 41, uint8_t );

BspTest4( Classic, 2, TagTest, 2, 2, 1, uint16_t );
BspTest4( Classic, 4, TagTest, 4, 2, 1, uint16_t );
BspTest4( Classic, 8, TagTest, 8, 2, 1, uint16_t );
BspTest4( Classic, 16, TagTest, 16, 2, 1, uint16_t );
BspTest4( Classic, 32, TagTest, 32, 2, 1, uint16_t );
BspTest4( Classic, 8, TagTest, 8, 4, 3, uint16_t );
BspTest4( Classic, 16, TagTest, 16, 4, 3, uint16_t );
BspTest4( Classic, 32, TagTest, 32, 4, 3, uint16_t );
BspTest4( Classic, 8, TagTest, 8, 7, 3, uint16_t );
BspTest4( Classic, 16, TagTest, 16, 7, 3, uint16_t );
BspTest4( Classic, 32, TagTest, 32, 7, 3, uint16_t );
BspTest4( Classic, 32, TagTest, 32, 100, 41, uint16_t );

BspTest4( Classic, 2, TagTest, 2, 2, 1, uint32_t );
BspTest4( Classic, 4, TagTest, 4, 2, 1, uint32_t );
BspTest4( Classic, 8, TagTest, 8, 2, 1, uint32_t );
BspTest4( Classic, 16, TagTest, 16, 2, 1, uint32_t );
BspTest4( Classic, 32, TagTest, 32, 2, 1, uint32_t );
BspTest4( Classic, 8, TagTest, 8, 4, 3, uint32_t );
BspTest4( Classic, 16, TagTest, 16, 4, 3, uint32_t );
BspTest4( Classic, 32, TagTest, 32, 4, 3, uint32_t );
BspTest4( Classic, 8, TagTest, 8, 7, 3, uint32_t );
BspTest4( Classic, 16, TagTest, 16, 7, 3, uint32_t );
BspTest4( Classic, 32, TagTest, 32, 7, 3, uint32_t );
BspTest4( Classic, 32, TagTest, 32, 100, 41, uint32_t );

BspTest4( Classic, 2, TagTest, 2, 2, 1, uint64_t );
BspTest4( Classic, 4, TagTest, 4, 2, 1, uint64_t );
BspTest4( Classic, 8, TagTest, 8, 2, 1, uint64_t );
BspTest4( Classic, 16, TagTest, 16, 2, 1, uint64_t );
BspTest4( Classic, 32, TagTest, 32, 2, 1, uint64_t );
BspTest4( Classic, 8, TagTest, 8, 4, 3, uint64_t );
BspTest4( Classic, 16, TagTest, 16, 4, 3, uint64_t );
BspTest4( Classic, 32, TagTest, 32, 4, 3, uint64_t );
BspTest4( Classic, 8, TagTest, 8, 7, 3, uint64_t );
BspTest4( Classic, 16, TagTest, 16, 7, 3, uint64_t );
BspTest4( Classic, 32, TagTest, 32, 7, 3, uint64_t );
BspTest4( Classic, 32, TagTest, 32, 100, 41, uint64_t );

#endif