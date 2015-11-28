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