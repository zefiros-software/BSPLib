/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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
#ifndef __BSPLIB_BENCH_H__
#define __BSPLIB_BENCH_H__

#include "bspedupack.h"

#include <armadillo>
#include <numeric>

using namespace arma;

/*  This program measures p, r, g, and l of a BSP computer using bsp_put for communication. */

#define NITERS 10000        /* number of iterations */
#define MAXN 1024           /* maximum length of DAXPY computation */
#define MAXH 1024            /* maximum h in h-relation */
#define MEGA 1000000.0

#ifdef __GNUC__
#ifndef BSP_USE_ASM_BENCH
#define BSP_USE_ASM_BENCH
#endif
#endif

void leastsquares( int h0, int h1, double *t, double *g, double *l )
{
    /* This function computes the parameters g and l of the
       linear function T(h)= g*h+l that best fits
       the data points (h,t[h]) with h0 <= h <= h1. */

    double nh, sumt, sumth, sumh, sumhh, a;
    int h;

    nh = h1 - h0 + 1;
    /* Compute sums:
        sumt  =  sum of t[h] over h0 <= h <= h1
        sumth =         t[h]*h
        sumh  =         h
        sumhh =         h*h     */
    sumt = sumth = 0.0;

    for ( h = h0; h <= h1; h++ )
    {
        sumt  += t[h];
        sumth += t[h] * h;
    }

    sumh = ( h1 * h1 - h0 * h0 + h1 + h0 ) / 2;
    sumhh = ( h1 * ( h1 + 1 ) * ( 2 * h1 + 1 ) - ( h0 - 1 ) * h0 * ( 2 * h0 - 1 ) ) / 6;

    /* Solve      nh*l +  sumh*g =  sumt
                sumh*l + sumhh*g = sumth */
    if ( fabs( nh ) > fabs( sumh ) )
    {
        a = sumh / nh;
        /* subtract a times first eqn from second eqn */
        *g = ( sumth - a * sumt ) / ( sumhh - a * sumh );
        *l = ( sumt - sumh **g ) / nh;
    }
    else
    {
        a = nh / sumh;
        /* subtract a times second eqn from first eqn */
        *g = ( sumt - a * sumth ) / ( sumh - a * sumhh );
        *l = ( sumth - sumhh **g ) / sumh;
    }

} /* end leastsquares */

// void LeastSquares( int h0, int h1, std::vector< double > &t, double &g, double &l )
// {
//     /* This function computes the parameters g and l of the
//        linear function T(h)= g*h+l that best fits
//        the data points (h,t[h]) with h0 <= h <= h1. */

//     double nh = h1 - h0 + 1;

//     auto tVec = vec( t.data() + h0, h1 - h0, false );

//     double sumt = sum( tVec );
//     double sumth = sum( tVec % linspace( h0, h1, h1 - h0 ) );

//     double sumh = ( h1 * h1 - h0 * h0 + h1 + h0 ) / 2;
//     double sumhh = ( h1 * ( h1 + 1 ) * ( 2 * h1 + 1 ) - ( h0 - 1 ) * h0 * ( 2 * h0 - 1 ) ) / 6;

//     /* Solve      nh*l +  sumh*g =  sumt
//                 sumh*l + sumhh*g = sumth */
//     if ( fabs( nh ) > fabs( sumh ) )
//     {
//         /* subtract a times first eqn from second eqn */
//         double a = sumh / nh;
//         g = ( sumth - a * sumt ) / ( sumhh - a * sumh );
//         l = ( sumt - sumh * g ) / nh;
//     }
//     else
//     {
//         /* subtract a times second eqn from first eqn */
//         double a = nh / sumh;
//         g = ( sumt - a * sumth ) / ( sumh - a * sumhh );
//         l = ( sumth - sumhh * g ) / sumh;
//     }
// }

void OrdinaryLeastSquares( int h0, int h1, std::vector< double > &t, double &g, double &l )
{
    vec endogenous( t.data() + h0, h1 - h0, false );
    vec exogenous  = linspace( h0, h1, h1 - h0 );

    double meanEnd = mean( endogenous );
    double meanEx = ( h1 + h0 ) / 2.0; //mean( exogenous );

    auto adjEx = exogenous - meanEx;
    auto adjEnd = endogenous - meanEnd;

    g = as_scalar( sum( adjEx % adjEnd ) / sum( square( adjEx ) ) );
    l = meanEnd - g * meanEx;
}

void ReportResults( double &g0, double &l0, uint32_t p, double r, double &g, double &l, std::vector< double > &t )
{
    //*
    OrdinaryLeastSquares( 0, p, t, g0, l0 );
    OrdinaryLeastSquares( p, MAXH, t, g, l );
    /*/
    leastsquares( 0, p, t.data(), &g0, &l0 );
    leastsquares( p, MAXH, t.data(), &g, &l );
    /**/

    printf( "size of double = %d bytes\n", ( uint32_t )sizeof( double ) );
    printf( "Range h=0 to p   : g= %.1lf, l= %.1lf\n", g0, l0 );
    printf( "Range h=p to HMAX: g= %.1lf, l= %.1lf\n", g, l );

    printf( "The bottom line for this BSP computer is:\n" );
    printf( "p= %d, r= %.3lf Mflop/s, g= %.1lf,\t\tl= %.1lf\n", p, r / MEGA, g, l );
    printf( "p= %d, r= %.3lf Mflop/s, g= %.5lf r,\tl= %.5lf r\n", p, r / MEGA, g / r * MEGA, l / r * MEGA );
    fflush( stdout );
}

void ReportH( std::vector<double> &t, std::vector<double> &Time, double r, uint32_t h )
{
    double time = mean( vec( Time.data(), Time.size(), false ) );
    t[h] = ( time * r ) / NITERS;

    printf( "Time of %5d-relation= %lf ms= %8.0lf flops\n", h, time * 1000 / NITERS, t[h] );
    fflush( stdout );
}

void ReportR( uint32_t n, double &r, std::vector<double> &Time, double *y, double *z )
{
    double mintime = *std::min_element( Time.begin(), Time.end() );
    double maxtime = *std::max_element( Time.begin(), Time.end() );

    if ( mintime > 0.0 )
    {
        /* Compute r = average computing rate in flop/s */
        double nflops = 4 * NITERS * n;
        r = mean( nflops / vec( Time.data(), Time.size(), false ) );

        printf( "n= %5d min= %7.3lf max= %7.3lf av= %7.3lf Mflop/s ", n, nflops / ( maxtime * MEGA ),
                nflops / ( mintime * MEGA ), r / MEGA );
        fflush( stdout );
        /* Output for fooling benchmark-detecting compilers */
        printf( " fool=%7.1lf\n", y[n - 1] + z[n - 1] );
    }
    else
    {
        printf( "minimum time is 0\n" );
    }

    fflush( stdout );
}

void  MeasureR( double *x, double *y, double *z, double beta, double alpha, int n )
{
#ifdef BSP_USE_ASM_BENCH
    {
        __asm__ __volatile__( "                         \
                    xor %%rax, %%rax;                   \
                    mov %[n], %%ecx;                    \
                    loop_begin_y%=:                     \
                        movsd (%[y], %%rax, 8), %%xmm0; \
                        movsd (%[x], %%rax, 8), %%xmm1; \
                        movsd %[alpha], %%xmm2;         \
                        mulsd %%xmm1, %%xmm2;           \
                        addsd %%xmm2, %%xmm0;           \
                        movsd %%xmm0, (%0, %%rax, 8);   \
                        inc %%rax;                      \
                        dec %%ecx;                      \
                        jnz loop_begin_y%=;" : "=r"( y ) : [y]"r"( y ), [x]"r"( x ), [alpha]"m"( alpha ), [n]"m"( n ) : "xmm0", "xmm1", "xmm2", "rax", "ecx" );
    }
#else

    for ( int i = 0; i < n; ++i )
    {
        y[i] += alpha * x[i];
    }

#endif

#ifdef BSP_USE_ASM_BENCH
    {
        __asm__ __volatile__( "                         \
                    xor %%rax, %%rax;                   \
                    mov %[n], %%ecx;                    \
                    loop_begin_z%=:                     \
                        movsd (%[z], %%rax, 8), %%xmm0; \
                        movsd (%[x], %%rax, 8), %%xmm1; \
                        movsd %[beta], %%xmm2;          \
                        mulsd %%xmm1, %%xmm2;           \
                        subsd %%xmm2, %%xmm0;           \
                        movsd %%xmm0, (%0, %%rax, 8);   \
                        inc %%rax;                      \
                        dec %%ecx;                      \
                        jnz loop_begin_z%=;" : "=r"( z ) : [z]"r"( z ), [x]"r"( x ), [beta]"m"( beta ), [n]"m"( n ) : "xmm0", "xmm1", "xmm2", "rax", "ecx" );
    }
#else

    for ( int i = 0; i < n; ++i )
    {
        z[i] -= beta * x[i];
    }

#endif
}

void  CorrectionR( double * /*x*/, double * /*y*/, double * /*z*/, double /*beta*/, double /*alpha*/, int n )
{
#ifdef BSP_USE_ASM_BENCH
    {
        __asm__ __volatile__( "                         \
                    xor %%rax, %%rax;                   \
                    mov %[n], %%ecx;                    \
                    correction_loop_begin_y%=:          \
                        inc %%rax;                      \
                        dec %%ecx;                      \
                        jnz correction_loop_begin_y%=;" : "=r"( y ) : [y]"r"( y ), [x]"r"( x ), [alpha]"m"( alpha ), [n]"m"( n ) : "xmm0", "xmm1", "xmm2", "rax", "ecx" );
    }
#else

    for ( int i = 0; i < n; ++i )
    {
        //y[i] += alpha * y[i];
    }

#endif

#ifdef BSP_USE_ASM_BENCH
    {
        __asm__ __volatile__( "                         \
                    xor %%rax, %%rax;                   \
                    mov %[n], %%ecx;                    \
                    correction_loop_begin_z%=:          \
                        inc %%rax;                      \
                        dec %%ecx;                      \
                        jnz correction_loop_begin_z%=;" : "=r"( z ) : [z]"r"( z ), [x]"r"( x ), [beta]"m"( beta ), [n]"m"( n ) : "xmm0", "xmm1", "xmm2", "rax", "ecx" );
    }
#else

    for ( int i = 0; i < n; ++i )
    {
        //z[i] -= beta * x[i];
    }

#endif
}

void DetermineR( double &r, std::vector<double> &Time, uint32_t s )
{
    double alpha, beta, x[MAXN], y[MAXN], z[MAXN], time;

    for ( uint32_t n = 1; n <= MAXN; n *= 2 )
    {
        /* Initialize scalars and vectors */
        alpha = 1.0 / 3.0;
        beta = 4.0 / 9.0;

        for ( uint32_t i = 0; i < n; i++ )
        {
            z[i] = y[i] = x[i] = ( double )i;
        }

        /* Measure time of 2*NITERS DAXPY operations of length n */
        BSPLib::Tic();

        for ( uint32_t iter = 0; iter < NITERS * 10; ++iter )
        {
            MeasureR( x, y, z, beta, alpha, n );
        }

        time = BSPLib::Toc() / 10.0;

        BSPLib::Tic();

        for ( uint32_t iter = 0; iter < NITERS * 10; ++iter )
        {
            CorrectionR( x, y, z, beta, alpha, n );
        }

        time -= BSPLib::Toc() / 10.0;

        BSPLib::PutIterator( 0, &time, 1, Time.begin(), s );
        BSPLib::Sync();

        /* Processor 0 determines minimum, maximum, average computing rate */
        if ( s == 0 )
        {
            ReportR( n, r, Time, y, z );
        }
    }
}

template< typename tFunc >
void DetermineGL( double r, std::vector<double> &t, std::vector<double> &dest, std::vector<double> &Time, uint32_t p,
                  uint32_t s, const tFunc &measureCommunication )
{
    uint32_t destproc[MAXH], destindex[MAXH];
    double time, src[MAXH];

    /**** Determine g and l ****/
    for ( uint32_t h = 0; h <= MAXH; ++h )
    {
        /* Initialize communication pattern */
        for ( uint32_t i = 0; i < h; ++i )
        {
            src[i] = ( double )i;

            if ( p == 1 )
            {
                destproc[i] = 0;
                destindex[i] = i;
            }
            else
            {
                /* destination processor is one of the p-1 others */
                destproc[i] = ( s + 1 + i % ( p - 1 ) ) % p;
                /* destination index is in my own part of dest */
                destindex[i] = s + ( i / ( p - 1 ) ) * p;
            }
        }

        /* Measure time of NITERS h-relations */
        BSPLib::SyncPoint();

        BSPLib::Tic();

        measureCommunication( destproc, destindex, src, dest, h );

        time = BSPLib::Toc();

        //bsp_put( 0, &time, Time, s * SZDBL, SZDBL );
        BSPLib::PutIterator( 0, &time, 1, Time.begin(), s );
        BSPLib::SyncPutRequests();

        /* Compute time of one h-relation */
        if ( s == 0 )
        {
            ReportH( t, Time, r, h );
        }
    }
}

void MeasurePut( uint32_t *destproc, uint32_t *destindex, double *src, std::vector<double> &dest, uint32_t h )
{
    for ( uint32_t iter = 0; iter < NITERS; ++iter )
    {
        for ( uint32_t i = 0; i < h; ++i )
        {
            BSPLib::PutIterator( destproc[i], src + i, 1, dest.begin(), destindex[i] );
        }

        BSPLib::SyncPutRequests();
    }
}

void MeasureGet( uint32_t *destproc, uint32_t *destindex, double *src, std::vector<double> &dest, uint32_t h )
{
    for ( uint32_t iter = 0; iter < NITERS; ++iter )
    {
        for ( uint32_t i = 0; i < h; ++i )
        {
            BSPLib::GetIterator( destproc[i], dest.begin(), destindex[i], src + i, 1 );
        }

        BSPLib::SyncGetRequests();
    }
}

void MeasureSend( uint32_t *destproc, uint32_t *destindex, double *src, std::vector<double> &dest, uint32_t h )
{
    BSPLib::SetTagsize<uint32_t>();
    BSPLib::Sync();

    for ( uint32_t iter = 0; iter < NITERS; ++iter )
    {
        for ( uint32_t i = 0; i < h; ++i )
        {
            BSPLib::Send( destproc[i], destindex[i], src[i] );
        }

        BSPLib::Sync();

        uint32_t tag;
        size_t status;

        for ( uint32_t i = 0; i < h; ++i )
        {
            BSPLib::GetTag( status, tag );
            BSPLib::Move( dest[tag] );
        }
    }
}

#endif