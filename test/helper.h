/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2015 Koen Visscher, Paul Visscher and individual contributors.
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
 *
 * @endcond
 */

#pragma once
#ifndef __PREDICTION_HELPER_H__
#define __PREDICTION_HELPER_H__

#define CONCATEXT( a, b ) a##b
#define CONCAT( a, b ) CONCATEXT( a, b )
#define PX( prefix ) CONCAT( PREFIX, prefix )

#ifdef __INTEL_COMPILER

#define __builtin_huge_val() HUGE_VAL
#define __builtin_huge_valf() HUGE_VALF
#define __builtin_nan nan
#define __builtin_nanf nanf
#define __builtin_nans nan
#define __builtin_nansf nanf

#endif

#define ARMA_USE_CXX11
#include <armadillo>

#define PLOTLIB_ARMA
#include "plot/plotting.h"

#include <gtest/gtest.h>

#include <unordered_set>

using namespace arma;

template<typename T, size_t size>
::testing::AssertionResult ArraysMatch( const T( &expected )[size],
                                        const T( &actual )[size] )
{
    for ( size_t i = 0; i < size; ++i )
    {
        if ( expected[i] != actual[i] )
        {
            return ::testing::AssertionFailure() << "array[" << i
                   << "] (" << actual[i] << ") != expected[" << i
                   << "] (" << expected[i] << ")";
        }
    }

    return ::testing::AssertionSuccess();
}

#endif