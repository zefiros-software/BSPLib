/**
 * Copyright (c) 2017 Zefiros Software.
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
#ifndef __BSPLIB_UTIL_H__
#define __BSPLIB_UTIL_H__

// From boost. Forces inlining on multiple compilers for extra perfomance.
#if !defined(BSP_FORCEINLINE)
#  if defined(_MSC_VER)
#    define BSP_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define BSP_FORCEINLINE inline __attribute__ ((__always_inline__))
#  else
#    define BSP_FORCEINLINE inline
#  endif
#endif


#if !defined(BSP_TLS)
#  if defined(_MSC_VER)
#    define BSP_TLS __declspec(thread)
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define BSP_TLS __thread
#  else
#    error "Define a thread local storage qualifier for your compiler/platform!"
#  endif
#endif

#include <chrono>

namespace BSPUtil
{
    template< typename tLoopIterator, typename tFunc>
    BSP_FORCEINLINE void SplitFor( tLoopIterator begin, tLoopIterator end, tLoopIterator start, const tFunc &body )
    {
        for ( tLoopIterator it = start; it < end; ++it )
        {
            body( it );
        }

        for ( tLoopIterator it = begin; it < start; ++it )
        {
            body( it );
        }
    }

    template< bool tCondition >
    class StaticIfUnreached
    {
    public:

        template< typename tFuncIf >
        StaticIfUnreached( const tFuncIf & )
        {
        }

        template< bool tElseIfCondition, typename tFuncElseIf >
        StaticIfUnreached< tElseIfCondition > ElseIf( const tFuncElseIf &elseIfBody )
        {
            return StaticIfUnreached< tElseIfCondition >( elseIfBody );
        }

        template< typename tFuncElse >
        void Else( const tFuncElse & )
        {
        }
    };

    template< bool tCondition >
    class StaticIf
    {
    public:

        template< typename tFuncIf >
        StaticIf( const tFuncIf &ifBody )
        {
            ifBody();
        }

        template< bool tElseIfCondition, typename tFuncElseIf >
        StaticIfUnreached< tElseIfCondition > ElseIf( const tFuncElseIf &elseIfBody )
        {
            return StaticIfUnreached< tElseIfCondition >( elseIfBody );
        }

        template< typename tFuncElse >
        void Else( const tFuncElse & )
        {
        }
    };

    template<>
    class StaticIf<false>
    {
    public:

        template< typename tFuncIf >
        StaticIf( const tFuncIf & )
        {
        }

        template< bool tElseIfCondition, typename tFuncElseIf >
        StaticIf< tElseIfCondition > ElseIf( const tFuncElseIf &elseIfBody )
        {
            return StaticIf< tElseIfCondition >( elseIfBody );
        }

        template< typename tFuncElse >
        void Else( const tFuncElse &elseBody )
        {
            elseBody();
        }
    };

    class TicTimer
    {
    public:

        TicTimer()
            : mTicTime( std::chrono::high_resolution_clock::now() )
        {

        }

        void Tic()
        {
            mTicTime = std::chrono::high_resolution_clock::now();
        }

        double Toc()
        {
            const std::chrono::time_point< std::chrono::high_resolution_clock > now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = now - mTicTime;
            return diff.count();
        }

        double TocTic()
        {
            double diff = Toc();
            Tic();
            return diff;
        }

    private:

        std::chrono::time_point< std::chrono::high_resolution_clock > mTicTime;
    };
}

#endif