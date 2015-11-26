#pragma once
#ifndef __HELPER_H__
#define __HELPER_H__

#include "bsp/bsp.h"

#include "gtest/gtest.h"

#define CONCATEXT( a, b ) a##b
#define CONCAT( a, b ) CONCATEXT( a, b )
#define P( prefix ) CONCAT( PREFIX, prefix )



#define BspTest( suite, nProc, func )       \
TEST( P( suite ), func ## _ ## nProc )      \
{                                           \
    BspExecute( func, nProc );              \
}

#define BspTest1( suite, nProc, func, a )               \
TEST( P( suite ), func ## _ ## nProc ## _ ## a )        \
{                                                       \
    BspExecute( func< a >, nProc );                     \
}

#define BspTest2( suite, nProc, func, a, b )                    \
TEST( P( suite ), func ## _ ## nProc ## _ ## a ## _ ## b )      \
{                                                               \
    BspExecute( func< a, b >, nProc );                          \
}

#define BspTest3( suite, nProc, func, a, b, c )                         \
TEST( P( suite ), func ## _ ## nProc ## _ ## a ## _ ## b ## _ ## c )    \
{                                                                       \
    BspExecute( func< a, b, c >, nProc );                               \
}

#define BspTest4( suite, nProc, func, a, b, c, d )                                  \
TEST( P( suite ), func ## _ ## nProc ## _ ## a ## _ ## b ## _ ## c ## _ ## d )      \
{                                                                                   \
    BspExecute( func< a, b, c, d >, nProc );                                        \
}

inline void BspExecute( std::function< void() > func, uint32_t nProc )
{
    auto spmd = [func, nProc]
    {
        BSPLib::Begin( nProc );

        func();

        BSPLib::End();
    };

    BSPLib::Init( spmd, 0, NULL );

    spmd();
}

#endif