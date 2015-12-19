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
#ifndef __TESTCLASSIC_H__
#define __TESTCLASSIC_H__

#include "helper.h"
#include <random>
#include <stdarg.h>

inline void AbortTest()
{
    BSPLib::Sync();
    BSPLib::Classic::Abort( "" );
}

TEST( P( Classic ), AbortTest )
{
    EXPECT_FALSE( BSPLib::Execute( AbortTest, 8 ) );

}

inline void VAbortTest()
{
    BSPLib::Sync();
    []( const char *format, ... )
    {
        va_list args;
        va_start( args, format );

        //pass to bsp_vabort

        BSPLib::Classic::VAbort( format, args );

        //mark end of variable arguments
        va_end( args );
    }( "", "test" );
}

TEST( P( Classic ), VAbortTest )
{
    EXPECT_FALSE( BSPLib::Execute( VAbortTest, 32 ) );

}

inline void AbortTestMain()
{
    BSPLib::Sync();

    if ( !BSPLib::ProcId() )
    {
        BSPLib::Classic::Abort( "" );
    }
}

TEST( P( Classic ), AbortTestMain )
{
    EXPECT_FALSE( BSPLib::Execute( AbortTestMain, 8 ) );
}

inline void AbortTestFirst()
{
    BSPLib::Sync();

    if ( BSPLib::ProcId() == 1 )
    {
        BSPLib::Classic::Abort( "" );
    }

    BSPLib::Sync();
}

TEST( P( Classic ), AbortTestFirst )
{
    EXPECT_FALSE( BSPLib::Execute( AbortTestFirst, 8 ) );
}

inline void AbortTestAllWaitExceptOne()
{
    BSPLib::Sync();

    if ( BSPLib::ProcId() == 1 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        BSPLib::Classic::Abort( "" );
    }

    BSPLib::Sync();
}

TEST( P( Classic ), AbortTestAllWaitExceptOne )
{
    EXPECT_FALSE( BSPLib::Execute( AbortTestAllWaitExceptOne, 8 ) );
}

inline void AbortTestAllWaitExceptMain()
{
    BSPLib::Sync();

    if ( BSPLib::ProcId() == 0 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        BSPLib::Classic::Abort( "" );
    }

    BSPLib::Sync();
}

TEST( P( Classic ), AbortTestAllWaitExceptMain )
{
    EXPECT_FALSE( BSPLib::Execute( AbortTestAllWaitExceptMain, 8 ) );
}

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
        BSPLib::Classic::Push( &reg, sizeof( uint32_t ) );
        BSPLib::Sync();

        BSPLib::Classic::Pop( &reg );
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

    BSPLib::Classic::Push( &receive, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tPuts; ++i )
    {
        BSPLib::Classic::Put( to, &num, &receive, 0, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }

    BSPLib::Classic::Pop( &receive );
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

    // Test new function that passes through to classic
    BSPLib::Push( &num, sizeof( uint32_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tGets; ++i )
    {
        BSPLib::Classic::Get( from, &num, 0, &receive, sizeof( uint32_t ) );

        BSPLib::Sync();

        EXPECT_EQ( expected, receive );

        receive = 0;
    }

    // Test new function that passes through to classic
    BSPLib::Pop( &num );
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


    BSPLib::Classic::Push( &num, sizeof( uint32_t ) );
    BSPLib::Classic::Push( &received, sizeof( uint32_t ) );

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

    BSPLib::Classic::Pop( &received );
    BSPLib::Classic::Pop( &num );
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
    std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
    double end = BSPLib::Time();

    EXPECT_GT( end - start, 0.05 );
}

template< uint32_t tRounds, uint32_t tPacketCount, uint32_t tPacketSize, int32_t tOffset >
void QSizeTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;

    uint32_t packets[tPacketSize];

    size_t packetCount;
    size_t totalSize;

    for ( uint32_t i = 0; i < tRounds; ++i )
    {
        for ( uint32_t j = 0; j < tPacketCount; ++j )
        {
            BSPLib::Classic::Send( sSend, NULL, packets, tPacketSize * sizeof( uint32_t ) );
        }

        BSPLib::Sync();

        BSPLib::Classic::QSize( &packetCount, &totalSize );

        EXPECT_EQ( tPacketCount, packetCount );
        EXPECT_EQ( tPacketCount * tPacketSize * sizeof( uint32_t ), totalSize );
    }
}

template< uint32_t tRounds, uint32_t tPacketCount, uint32_t tPacketSize, int32_t tOffset >
void QSizeTestOverload()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;

    uint32_t packets[tPacketSize];

    size_t packetCount;
    size_t totalSize;

    for ( uint32_t i = 0; i < tRounds; ++i )
    {
        for ( uint32_t j = 0; j < tPacketCount; ++j )
        {
            BSPLib::Classic::Send( sSend, NULL, packets, tPacketSize * sizeof( uint32_t ) );
        }

        BSPLib::Sync();

        BSPLib::QSize( packetCount, totalSize );

        EXPECT_EQ( tPacketCount, packetCount );
        EXPECT_EQ( tPacketCount * tPacketSize * sizeof( uint32_t ), totalSize );
    }
}

template< uint32_t tRounds, uint32_t tPacketCount, uint32_t tPacketSize, int32_t tOffset >
void QSizeTestOverload2()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;

    uint32_t packets[tPacketSize];

    size_t packetCount;
    //size_t totalSize;

    for ( uint32_t i = 0; i < tRounds; ++i )
    {
        for ( uint32_t j = 0; j < tPacketCount; ++j )
        {
            BSPLib::Classic::Send( sSend, NULL, packets, tPacketSize * sizeof( uint32_t ) );
        }

        BSPLib::Sync();

        BSPLib::QSize( packetCount );

        EXPECT_EQ( tPacketCount, packetCount );
    }
}

template< uint32_t tRounds >
void MultiSendTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    size_t tagSize = sizeof( uint32_t );

    BSPLib::Classic::SetTagSize( &tagSize );

    uint32_t message = s + 1;

    std::vector< bool > haveReceived( nProc, false );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tRounds; ++i )
    {
        for ( uint32_t sOther = 0; sOther < nProc; ++sOther )
        {
            BSPLib::Classic::Send( sOther, &s, &message, sizeof( uint32_t ) );
        }

        BSPLib::Sync();

        size_t packets;
        BSPLib::Classic::QSize( &packets, NULL );

        size_t status;
        uint32_t tag;
        uint32_t mail;

        for ( uint32_t j = 0; j < packets; ++j )
        {
            BSPLib::Classic::GetTag( &status, &tag );
            haveReceived[tag] = true;
            BSPLib::Classic::Move( &mail, sizeof( uint32_t ) );

            EXPECT_EQ( tag + 1, mail );
        }

        for ( auto received : haveReceived )
        {
            EXPECT_TRUE( received );
        }

        std::fill( haveReceived.begin(), haveReceived.end(), false );
    }
}

template< uint32_t tRounds, uint32_t tMaxSize, int32_t tOffset, typename tPrimitive >
void BruteForceTest()
{
    std::mt19937 engine;
    std::uniform_int_distribution< tPrimitive > dist( 0, tMaxSize );

    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sTarget = ( s + tOffset + nProc ) % nProc;
    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    engine.seed( s );

    std::vector< tPrimitive > putBuffer;
    std::vector< tPrimitive > putTarget;

    std::vector< tPrimitive > getBuffer;
    std::vector< tPrimitive > getTarget;

    std::vector< tPrimitive > sendBuffer;
    std::vector< tPrimitive > sendTarget;

    size_t putBufferSize = 0;
    size_t putTargetSize = 0;

    size_t getBufferSize = 0;
    size_t getTargetSize = 0;

    size_t sendBufferSize = 0;
    size_t sendTargetSize = 0;

    size_t status = 0;
    size_t tagSize = sizeof( size_t );

    BSPLib::Classic::SetTagSize( &tagSize );

    // Register the location where the size of the put vector can be placed
    BSPLib::Classic::Push( &putTargetSize, sizeof( size_t ) );


    // Register the location where the size of the get vector can be retrieved
    BSPLib::Classic::Push( &getTargetSize, sizeof( size_t ) );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tRounds; ++i )
    {
        // Resize the vectors to a random size. Use the local random generator for thread-safety
        putBufferSize = dist( engine );
        getTargetSize = dist( engine );
        sendBufferSize = dist( engine );

        putBuffer.resize( putBufferSize, ( tPrimitive )( s + 1 ) );
        getTarget.resize( getTargetSize, ( tPrimitive )( s + 1 ) );
        sendBuffer.resize( sendBufferSize, ( tPrimitive )( s + 1 ) );

        // Register where the other processor can get the data from
        BSPLib::Classic::Push( getTarget.data(), getTarget.size() * sizeof( tPrimitive ) );

        // Get the size the other processor has prepared
        BSPLib::Classic::Get( sTarget, &getTargetSize, 0, &getBufferSize, sizeof( size_t ) );

        // Put the size we want for the "Put" operation in the target process
        BSPLib::Classic::Put( sTarget, &putBufferSize, &putTargetSize, 0, sizeof( size_t ) );

        // Already send the data we want to send
        BSPLib::Classic::Send( sTarget, &sendBufferSize, sendBuffer.data(), sendBufferSize * sizeof( tPrimitive ) );

        BSPLib::Sync();

        // Prepare the target for the "Put" operation for the other process
        putTarget.resize( putTargetSize );

        // Register the vector with the new size
        BSPLib::Classic::Push( putTarget.data(), putTarget.size() * sizeof( tPrimitive ) );

        // Resize our own get buffer so that we can get the data from the other process
        getBuffer.resize( getBufferSize );

        // Get the data from the other process
        BSPLib::Classic::Get( sTarget, getTarget.data(), 0, getBuffer.data(), getBufferSize * sizeof( tPrimitive ) );

        // We are now done with the "Get" operation, pop the vector at the end of this sync
        BSPLib::Classic::Pop( getTarget.data() );

        // Retrieve the size of the "Send" operation of the other process
        BSPLib::Classic::GetTag( &status, &sendTargetSize );

        // Resize our target for the "Send" operation and move the data into it
        sendTarget.resize( sendTargetSize );
        BSPLib::Classic::Move( sendTarget.data(), sendTarget.size() * sizeof( tPrimitive ) );

        BSPLib::Classic::Sync();

        // Put the data in the other process
        BSPLib::Classic::Put( sTarget, putBuffer.data(), putTarget.data(), 0, putBufferSize * sizeof( tPrimitive ) );

        // Already pop our own target, so that it is popped at the end of the same sync
        BSPLib::Classic::Pop( putTarget.data() );

        BSPLib::Classic::Sync();

        for ( auto putVal : putTarget )
        {
            EXPECT_EQ( sSource + 1, putVal );
        }

        for ( auto getVal : getBuffer )
        {
            EXPECT_EQ( sTarget + 1, getVal );
        }

        for ( auto sendVal : sendTarget )
        {
            EXPECT_EQ( sSource + 1, sendVal );
        }
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

BspTest4( Classic, 2, QSizeTest, 10, 10, 10, 1 );
BspTest4( Classic, 4, QSizeTest, 10, 10, 10, 1 );
BspTest4( Classic, 8, QSizeTest, 10, 10, 10, 1 );
BspTest4( Classic, 16, QSizeTest, 10, 10, 10, 1 );
BspTest4( Classic, 32, QSizeTest, 10, 10, 10, 1 );
BspTest4( Classic, 2, QSizeTest, 10, 10, 10, 7 );
BspTest4( Classic, 4, QSizeTest, 10, 10, 10, 7 );
BspTest4( Classic, 8, QSizeTest, 10, 10, 10, 7 );
BspTest4( Classic, 16, QSizeTest, 10, 10, 10, 7 );
BspTest4( Classic, 32, QSizeTest, 10, 10, 10, 7 );
BspTest4( Classic, 32, QSizeTest, 10, 100, 17, 41 );

BspTest4( Classic, 32, QSizeTestOverload, 10, 100, 17, 41 );
BspTest4( Classic, 32, QSizeTestOverload2, 10, 100, 17, 41 );

BspTest1( Classic, 2, MultiSendTest, 50 );
BspTest1( Classic, 4, MultiSendTest, 50 );
BspTest1( Classic, 8, MultiSendTest, 50 );
BspTest1( Classic, 16, MultiSendTest, 50 );
BspTest1( Classic, 32, MultiSendTest, 50 );

BspTest4( Classic, 8, BruteForceTest, 500, 800, 5, uint16_t );
BspTest4( Classic, 8, BruteForceTest, 500, 800, 5, uint32_t );
BspTest4( Classic, 8, BruteForceTest, 500, 800, 5, uint64_t );

#ifndef DEBUG

BspTest4( Classic, 8, BruteForceTest, 1000, 1800, 3, uint16_t );
BspTest4( Classic, 8, BruteForceTest, 1000, 1800, 3, uint32_t );
BspTest4( Classic, 8, BruteForceTest, 1000, 1800, 3, uint64_t );

BspTest4( Classic, 3, BruteForceTest, 500, 800, 2, uint16_t );
BspTest4( Classic, 3, BruteForceTest, 500, 800, 2, uint32_t );
BspTest4( Classic, 3, BruteForceTest, 500, 800, 2, uint64_t );

BspTest4( Classic, 3, BruteForceTest, 1000, 1800, 1, uint16_t );
BspTest4( Classic, 3, BruteForceTest, 1000, 1800, 1, uint32_t );
BspTest4( Classic, 3, BruteForceTest, 1000, 1800, 1, uint64_t );

BspTest4( Classic, 16, BruteForceTest, 500, 800, 5, uint16_t );
BspTest4( Classic, 16, BruteForceTest, 500, 800, 5, uint32_t );
BspTest4( Classic, 16, BruteForceTest, 500, 800, 5, uint64_t );

BspTest4( Classic, 16, BruteForceTest, 1000, 1800, 3, uint16_t );
BspTest4( Classic, 16, BruteForceTest, 1000, 1800, 3, uint32_t );
BspTest4( Classic, 16, BruteForceTest, 1000, 1800, 3, uint64_t );

#endif // !DEBUG

#endif