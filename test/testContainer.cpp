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

    BSPLib::PushContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );
    BSPLib::PopContainer( result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutVectorOverloadTest()
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

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
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

    BSPLib::PushContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );
    BSPLib::PopContainer( result );

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

    BSPLib::PushContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );
    BSPLib::PopContainer( result );

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

    BSPLib::PushContainer( result );

    BSPLib::Sync();

    BSPLib::PutContainer( sTarget, container, result );
    BSPLib::PopContainer( result );

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

    BSPLib::PushCArray( result );

    BSPLib::Sync();

    BSPLib::PutCArray( sTarget, container, result );
    BSPLib::PopCArray( result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void PutCArrayOverloadTest()
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

    BSPLib::PutCArray( sTarget, container );
    BSPLib::PopCArray( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
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

    BSPLib::PushCArray( result );

    BSPLib::Sync();

    BSPLib::PutCArray( sTarget, container, result );
    BSPLib::PopCArray( result );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetVectorTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetContainer( sSource, container, result );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetVectorOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetContainer( sSource, container, result );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetFPVectorTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > container( tCount );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::vector< tPrimitive > result( tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetContainer( sSource, container, result );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > container;

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::array< tPrimitive, tCount > result;
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetContainer( sSource, container, result );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetFPStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSource = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > container;

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        container[i] = ( tPrimitive )( s + 1 + i );
    }

    std::array< tPrimitive, tCount > result;
    std::fill_n( result.begin(), tCount, ( tPrimitive )s );

    BSPLib::PushContainer( container );

    BSPLib::Sync();

    BSPLib::GetContainer( sSource, container, result );
    BSPLib::PopContainer( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetCArrayTest()
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

    BSPLib::GetCArray( sSource, container, result );
    BSPLib::PopCArray( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetCArrayOverloadTest()
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

    BSPLib::GetCArray( sSource, container );
    BSPLib::PopCArray( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sSource + 1 + i ), container[i] );
    }
}

template< typename tPrimitive, uint32_t tCount, int32_t tOffset >
void GetFPCArrayTest()
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

    BSPLib::GetCArray( sSource, container, result );
    BSPLib::PopCArray( container );

    BSPLib::Sync();

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sSource + 1 + i ), result[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendVectorTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > message( tCount );
    std::vector< tPrimitive > mailbox( tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendContainer( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendVectorOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > message( tCount );
    std::vector< tPrimitive > mailbox( tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag = s;

    BSPLib::SetTagsize< uint32_t >();

    BSPLib::Sync();

    BSPLib::SendContainer( sSend, tag, message );

    BSPLib::Sync();

    size_t status = 0;

    BSPLib::GetTag( status, tag );

    EXPECT_EQ( sReceive, tag );

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendVectorOverloadTest2()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > message( tCount );
    std::vector< tPrimitive > mailbox( tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag = s;

    BSPLib::SetTagsize< uint32_t >();

    BSPLib::Sync();

    BSPLib::SendContainer( sSend, &tag, message );

    BSPLib::Sync();

    size_t status = 0;

    BSPLib::GetTag( status, tag );

    EXPECT_EQ( sReceive, tag );

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendVectorOverloadTest3()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > message( tCount );
    std::vector< tPrimitive > mailbox( tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag[tCount];
    std::fill_n( tag, tCount, s );

    BSPLib::SetTagsize< uint32_t >( tCount );

    BSPLib::Sync();

    BSPLib::SendContainerWithCArray( sSend, tag, message );

    BSPLib::Sync();

    size_t status = 0;

    BSPLib::GetTagCArray( status, tag );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( sReceive, tag[i] );
    }

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendFPVectorTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::vector< tPrimitive > message( tCount );
    std::vector< tPrimitive > mailbox( tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendContainer( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > message;
    std::array< tPrimitive, tCount > mailbox;

    std::fill_n( mailbox.begin(), tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendContainer( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendFPStdArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    std::array< tPrimitive, tCount > message;
    std::array< tPrimitive, tCount > mailbox;

    std::fill_n( mailbox.begin(), tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendContainer( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveContainer( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendCArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message[tCount];
    tPrimitive mailbox[tCount];

    std::fill_n( mailbox, tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendCArray( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveCArray( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendCArrayOverloadTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message[tCount];
    tPrimitive mailbox[tCount];

    std::fill_n( mailbox, tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag = s;

    BSPLib::SetTagsize< uint32_t >();

    BSPLib::Sync();

    BSPLib::SendCArray( sSend, tag, message );

    BSPLib::Sync();

    size_t status;
    BSPLib::GetTag( status, tag );

    EXPECT_EQ( sReceive, tag );

    BSPLib::MoveCArray( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendCArrayOverloadTest2()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message[tCount];
    tPrimitive mailbox[tCount];

    std::fill_n( mailbox, tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag = s;

    BSPLib::SetTagsize< uint32_t >();

    BSPLib::Sync();

    BSPLib::SendCArray( sSend, &tag, message );

    BSPLib::Sync();

    size_t status;
    BSPLib::GetTag( status, tag );

    EXPECT_EQ( sReceive, tag );

    BSPLib::MoveCArray( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendCArrayOverloadTest3()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message[tCount];
    tPrimitive mailbox[tCount];

    std::fill_n( mailbox, tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    uint32_t tag[tCount];
    std::fill_n( tag, tCount, s );

    BSPLib::SetTagsize< uint32_t >( tCount );

    BSPLib::Sync();

    BSPLib::SendCArrayWithCArray( sSend, tag, message );

    BSPLib::Sync();

    size_t status;
    BSPLib::GetTag( status, tag );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( sReceive, tag[i] );
    }

    BSPLib::MoveCArray( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tPrimitive, int32_t tCount, int32_t tOffset >
void SendFPCArrayTest()
{
    uint32_t s = BSPLib::ProcId();
    uint32_t nProc = BSPLib::NProcs();

    uint32_t sSend = ( s + tOffset + nProc ) % nProc;
    uint32_t sReceive = ( s - tOffset + nProc ) % nProc;

    tPrimitive message[tCount];
    tPrimitive mailbox[tCount];

    std::fill_n( mailbox, tCount, ( tPrimitive )s );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        message[i] = ( tPrimitive )( s + 1 + i );
    }

    BSPLib::SendCArray( sSend, message );

    BSPLib::Sync();

    BSPLib::MoveCArray( mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( ( tPrimitive )( sReceive + 1 + i ), mailbox[i] );
    }
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagVectorTest()
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

    BSPLib::Send( sSend, tagContainer, message );

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
void TagStdArrayTest()
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

    BSPLib::Send( sSend, tagContainer, message );

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
void TagCArrayTest()
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

    BSPLib::SendWithCArray( sSend, tagContainer, message );

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
void TagFPVectorTest()
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

    BSPLib::Send( sSend, tagContainer, message );

    BSPLib::Sync();

    BSPLib::GetTagContainer( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagFPStdArrayTest()
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

    BSPLib::Send( sSend, tagContainer, message );

    BSPLib::Sync();

    BSPLib::GetTagContainer( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
}

template< typename tTagPrimitive, int32_t tCount, int32_t tOffset >
void TagFPCArrayTest()
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

    BSPLib::SendWithCArray( sSend, tagContainer, message );

    BSPLib::Sync();

    BSPLib::GetTagCArray( status, receiveTag );
    BSPLib::Move( mailbox );

    EXPECT_EQ( expectedMail, mailbox );

    for ( uint32_t i = 0; i < tCount; ++i )
    {
        EXPECT_DOUBLE_EQ( expectedTag[i], receiveTag[i] );
    }

    EXPECT_EQ( sizeof( uint32_t ), status );
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

BspTest3( Container, 32, PutVectorOverloadTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, PutVectorTest, uint64_t, 10000, 27 );
#endif

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

#ifndef DEBUG
BspTest3( Container, 32, PutStdArrayTest, uint64_t, 10000, 27 );
#endif

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

BspTest3( Container, 32, PutCArrayOverloadTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, PutCArrayTest, uint64_t, 10000, 27 );
#endif

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






BspTest3( Container, 2, GetVectorTest, uint8_t, 20, 1 );
BspTest3( Container, 4, GetVectorTest, uint8_t, 20, 3 );
BspTest3( Container, 8, GetVectorTest, uint8_t, 20, 7 );
BspTest3( Container, 16, GetVectorTest, uint8_t, 20, 5 );
BspTest3( Container, 32, GetVectorTest, uint8_t, 20, 27 );
BspTest3( Container, 2, GetVectorTest, uint8_t, 50, 1 );
BspTest3( Container, 4, GetVectorTest, uint8_t, 50, 3 );
BspTest3( Container, 8, GetVectorTest, uint8_t, 50, 7 );
BspTest3( Container, 16, GetVectorTest, uint8_t, 50, 5 );
BspTest3( Container, 32, GetVectorTest, uint8_t, 50, 27 );

BspTest3( Container, 2, GetVectorTest, uint64_t, 20, 1 );
BspTest3( Container, 4, GetVectorTest, uint64_t, 20, 3 );
BspTest3( Container, 8, GetVectorTest, uint64_t, 20, 7 );
BspTest3( Container, 16, GetVectorTest, uint64_t, 20, 5 );
BspTest3( Container, 32, GetVectorTest, uint64_t, 20, 27 );
BspTest3( Container, 2, GetVectorTest, uint64_t, 50, 1 );
BspTest3( Container, 4, GetVectorTest, uint64_t, 50, 3 );
BspTest3( Container, 8, GetVectorTest, uint64_t, 50, 7 );
BspTest3( Container, 16, GetVectorTest, uint64_t, 50, 5 );
BspTest3( Container, 32, GetVectorTest, uint64_t, 50, 27 );

BspTest3( Container, 32, GetVectorTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, GetVectorOverloadTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, GetVectorTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, GetFPVectorTest, float, 20, 1 );
BspTest3( Container, 4, GetFPVectorTest, float, 20, 3 );
BspTest3( Container, 8, GetFPVectorTest, float, 20, 7 );
BspTest3( Container, 16, GetFPVectorTest, float, 20, 5 );
BspTest3( Container, 32, GetFPVectorTest, float, 20, 27 );
BspTest3( Container, 2, GetFPVectorTest, float, 50, 1 );
BspTest3( Container, 4, GetFPVectorTest, float, 50, 3 );
BspTest3( Container, 8, GetFPVectorTest, float, 50, 7 );
BspTest3( Container, 16, GetFPVectorTest, float, 50, 5 );
BspTest3( Container, 32, GetFPVectorTest, float, 50, 27 );

BspTest3( Container, 2, GetFPVectorTest, double, 20, 1 );
BspTest3( Container, 4, GetFPVectorTest, double, 20, 3 );
BspTest3( Container, 8, GetFPVectorTest, double, 20, 7 );
BspTest3( Container, 16, GetFPVectorTest, double, 20, 5 );
BspTest3( Container, 32, GetFPVectorTest, double, 20, 27 );
BspTest3( Container, 2, GetFPVectorTest, double, 50, 1 );
BspTest3( Container, 4, GetFPVectorTest, double, 50, 3 );
BspTest3( Container, 8, GetFPVectorTest, double, 50, 7 );
BspTest3( Container, 16, GetFPVectorTest, double, 50, 5 );
BspTest3( Container, 32, GetFPVectorTest, double, 50, 27 );

BspTest3( Container, 2, GetStdArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, GetStdArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, GetStdArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, GetStdArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, GetStdArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, GetStdArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, GetStdArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, GetStdArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, GetStdArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, GetStdArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, GetStdArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, GetStdArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, GetStdArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, GetStdArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, GetStdArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, GetStdArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, GetStdArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, GetStdArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, GetStdArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, GetStdArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, GetStdArrayTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, GetStdArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, GetFPStdArrayTest, float, 20, 1 );
BspTest3( Container, 4, GetFPStdArrayTest, float, 20, 3 );
BspTest3( Container, 8, GetFPStdArrayTest, float, 20, 7 );
BspTest3( Container, 16, GetFPStdArrayTest, float, 20, 5 );
BspTest3( Container, 32, GetFPStdArrayTest, float, 20, 27 );
BspTest3( Container, 2, GetFPStdArrayTest, float, 50, 1 );
BspTest3( Container, 4, GetFPStdArrayTest, float, 50, 3 );
BspTest3( Container, 8, GetFPStdArrayTest, float, 50, 7 );
BspTest3( Container, 16, GetFPStdArrayTest, float, 50, 5 );
BspTest3( Container, 32, GetFPStdArrayTest, float, 50, 27 );

BspTest3( Container, 2, GetFPStdArrayTest, double, 20, 1 );
BspTest3( Container, 4, GetFPStdArrayTest, double, 20, 3 );
BspTest3( Container, 8, GetFPStdArrayTest, double, 20, 7 );
BspTest3( Container, 16, GetFPStdArrayTest, double, 20, 5 );
BspTest3( Container, 32, GetFPStdArrayTest, double, 20, 27 );
BspTest3( Container, 2, GetFPStdArrayTest, double, 50, 1 );
BspTest3( Container, 4, GetFPStdArrayTest, double, 50, 3 );
BspTest3( Container, 8, GetFPStdArrayTest, double, 50, 7 );
BspTest3( Container, 16, GetFPStdArrayTest, double, 50, 5 );
BspTest3( Container, 32, GetFPStdArrayTest, double, 50, 27 );

BspTest3( Container, 2, GetCArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, GetCArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, GetCArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, GetCArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, GetCArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, GetCArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, GetCArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, GetCArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, GetCArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, GetCArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, GetCArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, GetCArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, GetCArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, GetCArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, GetCArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, GetCArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, GetCArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, GetCArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, GetCArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, GetCArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, GetCArrayTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, GetCArrayOverloadTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, GetCArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, GetFPCArrayTest, float, 20, 1 );
BspTest3( Container, 4, GetFPCArrayTest, float, 20, 3 );
BspTest3( Container, 8, GetFPCArrayTest, float, 20, 7 );
BspTest3( Container, 16, GetFPCArrayTest, float, 20, 5 );
BspTest3( Container, 32, GetFPCArrayTest, float, 20, 27 );
BspTest3( Container, 2, GetFPCArrayTest, float, 50, 1 );
BspTest3( Container, 4, GetFPCArrayTest, float, 50, 3 );
BspTest3( Container, 8, GetFPCArrayTest, float, 50, 7 );
BspTest3( Container, 16, GetFPCArrayTest, float, 50, 5 );
BspTest3( Container, 32, GetFPCArrayTest, float, 50, 27 );

BspTest3( Container, 2, GetFPCArrayTest, double, 20, 1 );
BspTest3( Container, 4, GetFPCArrayTest, double, 20, 3 );
BspTest3( Container, 8, GetFPCArrayTest, double, 20, 7 );
BspTest3( Container, 16, GetFPCArrayTest, double, 20, 5 );
BspTest3( Container, 32, GetFPCArrayTest, double, 20, 27 );
BspTest3( Container, 2, GetFPCArrayTest, double, 50, 1 );
BspTest3( Container, 4, GetFPCArrayTest, double, 50, 3 );
BspTest3( Container, 8, GetFPCArrayTest, double, 50, 7 );
BspTest3( Container, 16, GetFPCArrayTest, double, 50, 5 );
BspTest3( Container, 32, GetFPCArrayTest, double, 50, 27 );






BspTest3( Container, 2, SendVectorTest, uint8_t, 20, 1 );
BspTest3( Container, 4, SendVectorTest, uint8_t, 20, 3 );
BspTest3( Container, 8, SendVectorTest, uint8_t, 20, 7 );
BspTest3( Container, 16, SendVectorTest, uint8_t, 20, 5 );
BspTest3( Container, 32, SendVectorTest, uint8_t, 20, 27 );
BspTest3( Container, 2, SendVectorTest, uint8_t, 50, 1 );
BspTest3( Container, 4, SendVectorTest, uint8_t, 50, 3 );
BspTest3( Container, 8, SendVectorTest, uint8_t, 50, 7 );
BspTest3( Container, 16, SendVectorTest, uint8_t, 50, 5 );
BspTest3( Container, 32, SendVectorTest, uint8_t, 50, 27 );

BspTest3( Container, 2, SendVectorTest, uint64_t, 20, 1 );
BspTest3( Container, 4, SendVectorTest, uint64_t, 20, 3 );
BspTest3( Container, 8, SendVectorTest, uint64_t, 20, 7 );
BspTest3( Container, 16, SendVectorTest, uint64_t, 20, 5 );
BspTest3( Container, 32, SendVectorTest, uint64_t, 20, 27 );
BspTest3( Container, 2, SendVectorTest, uint64_t, 50, 1 );
BspTest3( Container, 4, SendVectorTest, uint64_t, 50, 3 );
BspTest3( Container, 8, SendVectorTest, uint64_t, 50, 7 );
BspTest3( Container, 16, SendVectorTest, uint64_t, 50, 5 );
BspTest3( Container, 32, SendVectorTest, uint64_t, 50, 27 );

BspTest3( Container, 32, SendVectorTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, SendVectorOverloadTest, uint64_t, 1000, 27 );
BspTest3( Container, 32, SendVectorOverloadTest2, uint64_t, 1000, 27 );
BspTest3( Container, 32, SendVectorOverloadTest3, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, SendVectorTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, SendFPVectorTest, float, 20, 1 );
BspTest3( Container, 4, SendFPVectorTest, float, 20, 3 );
BspTest3( Container, 8, SendFPVectorTest, float, 20, 7 );
BspTest3( Container, 16, SendFPVectorTest, float, 20, 5 );
BspTest3( Container, 32, SendFPVectorTest, float, 20, 27 );
BspTest3( Container, 2, SendFPVectorTest, float, 50, 1 );
BspTest3( Container, 4, SendFPVectorTest, float, 50, 3 );
BspTest3( Container, 8, SendFPVectorTest, float, 50, 7 );
BspTest3( Container, 16, SendFPVectorTest, float, 50, 5 );
BspTest3( Container, 32, SendFPVectorTest, float, 50, 27 );

BspTest3( Container, 2, SendFPVectorTest, double, 20, 1 );
BspTest3( Container, 4, SendFPVectorTest, double, 20, 3 );
BspTest3( Container, 8, SendFPVectorTest, double, 20, 7 );
BspTest3( Container, 16, SendFPVectorTest, double, 20, 5 );
BspTest3( Container, 32, SendFPVectorTest, double, 20, 27 );
BspTest3( Container, 2, SendFPVectorTest, double, 50, 1 );
BspTest3( Container, 4, SendFPVectorTest, double, 50, 3 );
BspTest3( Container, 8, SendFPVectorTest, double, 50, 7 );
BspTest3( Container, 16, SendFPVectorTest, double, 50, 5 );
BspTest3( Container, 32, SendFPVectorTest, double, 50, 27 );

BspTest3( Container, 2, SendStdArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, SendStdArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, SendStdArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, SendStdArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, SendStdArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, SendStdArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, SendStdArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, SendStdArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, SendStdArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, SendStdArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, SendStdArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, SendStdArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, SendStdArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, SendStdArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, SendStdArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, SendStdArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, SendStdArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, SendStdArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, SendStdArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, SendStdArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, SendStdArrayTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, SendStdArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, SendFPStdArrayTest, float, 20, 1 );
BspTest3( Container, 4, SendFPStdArrayTest, float, 20, 3 );
BspTest3( Container, 8, SendFPStdArrayTest, float, 20, 7 );
BspTest3( Container, 16, SendFPStdArrayTest, float, 20, 5 );
BspTest3( Container, 32, SendFPStdArrayTest, float, 20, 27 );
BspTest3( Container, 2, SendFPStdArrayTest, float, 50, 1 );
BspTest3( Container, 4, SendFPStdArrayTest, float, 50, 3 );
BspTest3( Container, 8, SendFPStdArrayTest, float, 50, 7 );
BspTest3( Container, 16, SendFPStdArrayTest, float, 50, 5 );
BspTest3( Container, 32, SendFPStdArrayTest, float, 50, 27 );

BspTest3( Container, 2, SendFPStdArrayTest, double, 20, 1 );
BspTest3( Container, 4, SendFPStdArrayTest, double, 20, 3 );
BspTest3( Container, 8, SendFPStdArrayTest, double, 20, 7 );
BspTest3( Container, 16, SendFPStdArrayTest, double, 20, 5 );
BspTest3( Container, 32, SendFPStdArrayTest, double, 20, 27 );
BspTest3( Container, 2, SendFPStdArrayTest, double, 50, 1 );
BspTest3( Container, 4, SendFPStdArrayTest, double, 50, 3 );
BspTest3( Container, 8, SendFPStdArrayTest, double, 50, 7 );
BspTest3( Container, 16, SendFPStdArrayTest, double, 50, 5 );
BspTest3( Container, 32, SendFPStdArrayTest, double, 50, 27 );

BspTest3( Container, 2, SendCArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, SendCArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, SendCArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, SendCArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, SendCArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, SendCArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, SendCArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, SendCArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, SendCArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, SendCArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, SendCArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, SendCArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, SendCArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, SendCArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, SendCArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, SendCArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, SendCArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, SendCArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, SendCArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, SendCArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, SendCArrayTest, uint64_t, 1000, 27 );

BspTest3( Container, 32, SendCArrayOverloadTest, uint64_t, 1000, 27 );
BspTest3( Container, 32, SendCArrayOverloadTest2, uint64_t, 1000, 27 );
BspTest3( Container, 32, SendCArrayOverloadTest3, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, SendCArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, SendFPCArrayTest, float, 20, 1 );
BspTest3( Container, 4, SendFPCArrayTest, float, 20, 3 );
BspTest3( Container, 8, SendFPCArrayTest, float, 20, 7 );
BspTest3( Container, 16, SendFPCArrayTest, float, 20, 5 );
BspTest3( Container, 32, SendFPCArrayTest, float, 20, 27 );
BspTest3( Container, 2, SendFPCArrayTest, float, 50, 1 );
BspTest3( Container, 4, SendFPCArrayTest, float, 50, 3 );
BspTest3( Container, 8, SendFPCArrayTest, float, 50, 7 );
BspTest3( Container, 16, SendFPCArrayTest, float, 50, 5 );
BspTest3( Container, 32, SendFPCArrayTest, float, 50, 27 );

BspTest3( Container, 2, SendFPCArrayTest, double, 20, 1 );
BspTest3( Container, 4, SendFPCArrayTest, double, 20, 3 );
BspTest3( Container, 8, SendFPCArrayTest, double, 20, 7 );
BspTest3( Container, 16, SendFPCArrayTest, double, 20, 5 );
BspTest3( Container, 32, SendFPCArrayTest, double, 20, 27 );
BspTest3( Container, 2, SendFPCArrayTest, double, 50, 1 );
BspTest3( Container, 4, SendFPCArrayTest, double, 50, 3 );
BspTest3( Container, 8, SendFPCArrayTest, double, 50, 7 );
BspTest3( Container, 16, SendFPCArrayTest, double, 50, 5 );
BspTest3( Container, 32, SendFPCArrayTest, double, 50, 27 );






BspTest3( Container, 2, TagVectorTest, uint8_t, 20, 1 );
BspTest3( Container, 4, TagVectorTest, uint8_t, 20, 3 );
BspTest3( Container, 8, TagVectorTest, uint8_t, 20, 7 );
BspTest3( Container, 16, TagVectorTest, uint8_t, 20, 5 );
BspTest3( Container, 32, TagVectorTest, uint8_t, 20, 27 );
BspTest3( Container, 2, TagVectorTest, uint8_t, 50, 1 );
BspTest3( Container, 4, TagVectorTest, uint8_t, 50, 3 );
BspTest3( Container, 8, TagVectorTest, uint8_t, 50, 7 );
BspTest3( Container, 16, TagVectorTest, uint8_t, 50, 5 );
BspTest3( Container, 32, TagVectorTest, uint8_t, 50, 27 );

BspTest3( Container, 2, TagVectorTest, uint64_t, 20, 1 );
BspTest3( Container, 4, TagVectorTest, uint64_t, 20, 3 );
BspTest3( Container, 8, TagVectorTest, uint64_t, 20, 7 );
BspTest3( Container, 16, TagVectorTest, uint64_t, 20, 5 );
BspTest3( Container, 32, TagVectorTest, uint64_t, 20, 27 );
BspTest3( Container, 2, TagVectorTest, uint64_t, 50, 1 );
BspTest3( Container, 4, TagVectorTest, uint64_t, 50, 3 );
BspTest3( Container, 8, TagVectorTest, uint64_t, 50, 7 );
BspTest3( Container, 16, TagVectorTest, uint64_t, 50, 5 );
BspTest3( Container, 32, TagVectorTest, uint64_t, 50, 27 );

BspTest3( Container, 32, TagVectorTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, TagVectorTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, TagFPVectorTest, float, 20, 1 );
BspTest3( Container, 4, TagFPVectorTest, float, 20, 3 );
BspTest3( Container, 8, TagFPVectorTest, float, 20, 7 );
BspTest3( Container, 16, TagFPVectorTest, float, 20, 5 );
BspTest3( Container, 32, TagFPVectorTest, float, 20, 27 );
BspTest3( Container, 2, TagFPVectorTest, float, 50, 1 );
BspTest3( Container, 4, TagFPVectorTest, float, 50, 3 );
BspTest3( Container, 8, TagFPVectorTest, float, 50, 7 );
BspTest3( Container, 16, TagFPVectorTest, float, 50, 5 );
BspTest3( Container, 32, TagFPVectorTest, float, 50, 27 );

BspTest3( Container, 2, TagFPVectorTest, double, 20, 1 );
BspTest3( Container, 4, TagFPVectorTest, double, 20, 3 );
BspTest3( Container, 8, TagFPVectorTest, double, 20, 7 );
BspTest3( Container, 16, TagFPVectorTest, double, 20, 5 );
BspTest3( Container, 32, TagFPVectorTest, double, 20, 27 );
BspTest3( Container, 2, TagFPVectorTest, double, 50, 1 );
BspTest3( Container, 4, TagFPVectorTest, double, 50, 3 );
BspTest3( Container, 8, TagFPVectorTest, double, 50, 7 );
BspTest3( Container, 16, TagFPVectorTest, double, 50, 5 );
BspTest3( Container, 32, TagFPVectorTest, double, 50, 27 );

BspTest3( Container, 2, TagStdArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, TagStdArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, TagStdArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, TagStdArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, TagStdArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, TagStdArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, TagStdArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, TagStdArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, TagStdArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, TagStdArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, TagStdArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, TagStdArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, TagStdArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, TagStdArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, TagStdArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, TagStdArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, TagStdArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, TagStdArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, TagStdArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, TagStdArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, TagStdArrayTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, TagStdArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, TagFPStdArrayTest, float, 20, 1 );
BspTest3( Container, 4, TagFPStdArrayTest, float, 20, 3 );
BspTest3( Container, 8, TagFPStdArrayTest, float, 20, 7 );
BspTest3( Container, 16, TagFPStdArrayTest, float, 20, 5 );
BspTest3( Container, 32, TagFPStdArrayTest, float, 20, 27 );
BspTest3( Container, 2, TagFPStdArrayTest, float, 50, 1 );
BspTest3( Container, 4, TagFPStdArrayTest, float, 50, 3 );
BspTest3( Container, 8, TagFPStdArrayTest, float, 50, 7 );
BspTest3( Container, 16, TagFPStdArrayTest, float, 50, 5 );
BspTest3( Container, 32, TagFPStdArrayTest, float, 50, 27 );

BspTest3( Container, 2, TagFPStdArrayTest, double, 20, 1 );
BspTest3( Container, 4, TagFPStdArrayTest, double, 20, 3 );
BspTest3( Container, 8, TagFPStdArrayTest, double, 20, 7 );
BspTest3( Container, 16, TagFPStdArrayTest, double, 20, 5 );
BspTest3( Container, 32, TagFPStdArrayTest, double, 20, 27 );
BspTest3( Container, 2, TagFPStdArrayTest, double, 50, 1 );
BspTest3( Container, 4, TagFPStdArrayTest, double, 50, 3 );
BspTest3( Container, 8, TagFPStdArrayTest, double, 50, 7 );
BspTest3( Container, 16, TagFPStdArrayTest, double, 50, 5 );
BspTest3( Container, 32, TagFPStdArrayTest, double, 50, 27 );

BspTest3( Container, 2, TagCArrayTest, uint8_t, 20, 1 );
BspTest3( Container, 4, TagCArrayTest, uint8_t, 20, 3 );
BspTest3( Container, 8, TagCArrayTest, uint8_t, 20, 7 );
BspTest3( Container, 16, TagCArrayTest, uint8_t, 20, 5 );
BspTest3( Container, 32, TagCArrayTest, uint8_t, 20, 27 );
BspTest3( Container, 2, TagCArrayTest, uint8_t, 50, 1 );
BspTest3( Container, 4, TagCArrayTest, uint8_t, 50, 3 );
BspTest3( Container, 8, TagCArrayTest, uint8_t, 50, 7 );
BspTest3( Container, 16, TagCArrayTest, uint8_t, 50, 5 );
BspTest3( Container, 32, TagCArrayTest, uint8_t, 50, 27 );

BspTest3( Container, 2, TagCArrayTest, uint64_t, 20, 1 );
BspTest3( Container, 4, TagCArrayTest, uint64_t, 20, 3 );
BspTest3( Container, 8, TagCArrayTest, uint64_t, 20, 7 );
BspTest3( Container, 16, TagCArrayTest, uint64_t, 20, 5 );
BspTest3( Container, 32, TagCArrayTest, uint64_t, 20, 27 );
BspTest3( Container, 2, TagCArrayTest, uint64_t, 50, 1 );
BspTest3( Container, 4, TagCArrayTest, uint64_t, 50, 3 );
BspTest3( Container, 8, TagCArrayTest, uint64_t, 50, 7 );
BspTest3( Container, 16, TagCArrayTest, uint64_t, 50, 5 );
BspTest3( Container, 32, TagCArrayTest, uint64_t, 50, 27 );

BspTest3( Container, 32, TagCArrayTest, uint64_t, 1000, 27 );

#ifndef DEBUG
BspTest3( Container, 32, TagCArrayTest, uint64_t, 10000, 27 );
#endif

BspTest3( Container, 2, TagFPCArrayTest, float, 20, 1 );
BspTest3( Container, 4, TagFPCArrayTest, float, 20, 3 );
BspTest3( Container, 8, TagFPCArrayTest, float, 20, 7 );
BspTest3( Container, 16, TagFPCArrayTest, float, 20, 5 );
BspTest3( Container, 32, TagFPCArrayTest, float, 20, 27 );
BspTest3( Container, 2, TagFPCArrayTest, float, 50, 1 );
BspTest3( Container, 4, TagFPCArrayTest, float, 50, 3 );
BspTest3( Container, 8, TagFPCArrayTest, float, 50, 7 );
BspTest3( Container, 16, TagFPCArrayTest, float, 50, 5 );
BspTest3( Container, 32, TagFPCArrayTest, float, 50, 27 );

BspTest3( Container, 2, TagFPCArrayTest, double, 20, 1 );
BspTest3( Container, 4, TagFPCArrayTest, double, 20, 3 );
BspTest3( Container, 8, TagFPCArrayTest, double, 20, 7 );
BspTest3( Container, 16, TagFPCArrayTest, double, 20, 5 );
BspTest3( Container, 32, TagFPCArrayTest, double, 20, 27 );
BspTest3( Container, 2, TagFPCArrayTest, double, 50, 1 );
BspTest3( Container, 4, TagFPCArrayTest, double, 50, 3 );
BspTest3( Container, 8, TagFPCArrayTest, double, 50, 7 );
BspTest3( Container, 16, TagFPCArrayTest, double, 50, 5 );
BspTest3( Container, 32, TagFPCArrayTest, double, 50, 27 );