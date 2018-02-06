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
#include "bspedupack.h"

#include <complex>

/****************** Sequential functions ********************************/
void UFft( double *x, uint32_t n, int sign, std::vector<double> &w )
{

    /* This sequential function computes the unordered discrete Fourier
       transform of a complex vector x of length n, stored in a real array
       of length 2n as pairs (Re x[j], Im x[j]), 0 <= j < n.
       n=2^m, m >= 0.
       If sign = 1, then the forward unordered dft FRx is computed;
       if sign =-1, the backward unordered dft conjg(F)Rx is computed,
       where F is the n by n Fourier matrix and R the n by n bit-reversal
       matrix. The output overwrites x.
       w is a table of n/2 complex weights, stored as pairs of reals,
          exp(-2*pi*i*j/n), 0 <= j < n/2,
       which must have been initialized before calling this function.
    */
#ifdef _MSC_VER
    unsigned long end;
    _BitScanReverse( &end, n | 1 );
    const uint32_t mEnd = static_cast< uint32_t >( end );
#else
    const uint32_t mEnd = 31 - __builtin_clz( n );
#endif

    for ( uint32_t m = 1; m <= mEnd; ++m )
    {
        // const uint32_t k = 1 << m;
        const uint32_t nk = n >> m;
        const uint32_t jEnd = 1 << ( m - 1 );

        for ( uint32_t r = 0; r < nk; ++r )
        {
            const uint32_t rk = r << ( m + 1 );

            #pragma omp simd

            for ( uint32_t j = 0; j < jEnd; ++j )
            {
                const uint32_t jprime = j << 1;
                const uint32_t jnk = jprime * nk;
                const double wr = w[jnk];
                const double wi = sign * w[ jnk + 1 ];

                const uint32_t j0 = rk + jprime;
                const uint32_t j1 = j0 + 1;
                const uint32_t j2 = j0 + ( 1 << m );
                const uint32_t j3 = j2 + 1;
                const double taur = wr * x[j2] - wi * x[j3];
                const double taui = wi * x[j2] + wr * x[j3];
                x[j2] = x[j0] - taur;
                x[j3] = x[j1] - taui;
                x[j0] += taur;
                x[j1] += taui;
            }
        }
    }

} /* end ufft */

void UFftInit( uint32_t n, std::vector<double> &w )
{

    /* This function initializes the n/2 weights to be used
       in a sequential radix-2 FFT of length n.
       n=2^m, m >= 0.
       w is a table of n/2 complex weights, stored as pairs of reals,
          exp(-2*pi*i*j/n), 0 <= j < n/2.
    */
    if ( n == 1 )
    {
        return;
    }

    const double theta = ( -2.0 * M_PI ) / n;
    w[0] = 1.0;
    w[1] = 0.0;

    if ( n == 4 )
    {
        w[2] =  0.0;
        w[3] = -1.0;
    }
    else if ( n >= 8 )
    {
        /* weights 1 .. n/8 */
        for ( uint32_t j = 1, jEnd = n >> 3; j <= jEnd; ++j )
        {
            const uint32_t j2 = j << 1;
            // std::complex<double> wComplex( 0, j * theta );
            // wComplex = std::exp( wComplex );
            // //w[j2] =   cos( j * theta );
            // w[j2] = wComplex.real();

            // //w[j2 + 1] = sin( j * theta );
            // w[j2 + 1] = wComplex.imag();
#ifdef _MSC_VER
            w[j2] = std::cos( j * theta );
            w[j2 + 1] = std::sin( j * theta );
#else
            __builtin_sincos( j * theta, &w[j2 + 1], &w[j2] );
#endif // _MSC_VER

        }

        /* weights n/8+1 .. n/4 */
        for ( uint32_t j = 0, jEnd = n >> 3, n4 = n >> 2; j < jEnd; ++j )
        {
            const uint32_t n4j2 = ( n4 - j ) << 1;
            const uint32_t j2 = j << 1;
            w[n4j2] =   -w[j2 + 1];
            w[n4j2 + 1] = -w[j2];
        }

        /* weights n/4+1 .. n/2-1 */
        for ( uint32_t j = 1, jEnd = n >> 2, n2 = n >> 1; j < jEnd; ++j )
        {
            const uint32_t n2j2 = ( n2 - j ) << 1;
            const uint32_t j2 = j << 1;
            w[n2j2] =   -w[j2];
            w[n2j2 + 1] =  w[j2 + 1];
        }
    }

} /* end UFftInit */

void Twiddle( std::vector<double> &x, uint32_t n, int sign, double *w )
{

    /* This sequential function multiplies a complex vector x
       of length n, stored as pairs of reals, componentwise
       by a complex vector w of length n, if sign=1, and
       by conjg(w), if sign=-1. The result overwrites x.
    */

    for ( uint32_t j = 0, jEnd = 2 * n; j < jEnd; j += 2 )
    {
        const uint32_t j1 = j + 1;
        const double wr = w[j];
        const double wi = sign * w[j1];

        const double xr = x[j];
        const double xi = x[j1];
        x[j] =  wr * xr - wi * xi;
        x[j1] = wi * xr + wr * xi;
    }

} /* end Twiddle */

void TwiddleInit( uint32_t n, double alpha, std::vector<uint32_t> &rho, double *w )
{

    /* This sequential function initializes the weight table w
       to be used in twiddling with a complex vector of length n,
       stored as pairs of reals.
       n=2^m, m >= 0.
       alpha is a real shift parameter.
       rho is the bit-reversal permutation of length n,
       which must have been initialized before calling this function.
       The output w is a table of n complex values, stored as pairs of reals,
          exp(-2*pi*i*rho(j)*alpha/n), 0 <= j < n.
    */
    const double theta = ( -2.0 * M_PI * alpha ) / n;

    for ( uint32_t j = 0; j < n; ++j )
    {
        const uint32_t j2 = j << 1;
        const double rhoTheta = rho[j] * theta;

        w[j2] = cos( rhoTheta );
        w[j2 + 1] = sin( rhoTheta );
    }

} /* end TwiddleInit */

void Permute( std::vector<double> &x, uint32_t n, std::vector<uint32_t> &sigma )
{

    /* This in-place sequential function permutes a complex vector x
       of length n >= 1, stored as pairs of reals, by the permutation sigma,
           y[j] = x[sigma[j]], 0 <= j < n.
       The output overwrites the vector x.
       sigma is a permutation of length n that must be decomposable
       into disjoint swaps.
    */

    for ( uint32_t j = 0; j < n; j++ )
    {
        if ( j < sigma[j] )
        {
            /* swap components j and sigma[j] */
            const uint32_t j0 = j << 1;
            const uint32_t j1 = j0 + 1;
            const uint32_t j2 = sigma[j] << 1;
            const uint32_t j3 = j2 + 1;
            const double tmpr = x[j0];
            const double tmpi = x[j1];
            x[j0] = x[j2];
            x[j1] = x[j3];
            x[j2] = tmpr;
            x[j3] = tmpi;
        }
    }
} /* end Permute */

void BitRevInit( uint32_t n, std::vector<uint32_t> &rho )
{

    /* This function initializes the bit-reversal permutation rho
       of length n, with n=2^m, m >= 0.
    */
    if ( n == 1 )
    {
        rho[0] = 0;
        return;
    }

#ifdef _MSC_VER
    unsigned long end;
    _BitScanReverse( &end, n | 1 );
    const uint32_t kEnd = static_cast<uint32_t>( end );
#else
    const uint32_t kEnd = 31 - __builtin_clz( n );
#endif

    for ( uint32_t j = 0; j < n; ++j )
    {
        uint32_t val = 0;

        for ( uint32_t k = 0; k < kEnd; ++k )
        {
            const uint32_t lastbit = ( j >> k ) & 0x1;
            val = ( val << 1 ) | lastbit;
        }

        rho[j] = val;
    }

} /* end BitRevInit */

/****************** Parallel functions ********************************/
int K1Init( uint32_t n, uint32_t p )
{

    /* This function computes the largest butterfly size k1 of the first
       superstep in a parallel FFT of length n on p processors with p < n.
    */
    uint32_t np = n / p;

    uint32_t c;

    for ( c = 1; c < p; c *= np );

    return n / c;

} /* end K1Init */

void BSPRedistr( std::vector<double> &x, uint32_t n, uint32_t p, uint32_t s, uint32_t c0, uint32_t c1,
                 bool rev, std::vector<uint32_t> &rho_p )
{

    /* This function redistributes the complex vector x of length n,
       stored as pairs of reals, from group-cyclic distribution
       over p processors with cycle c0 to cycle c1, where
       c0, c1, p, n are powers of two with 1 <= c0 <= c1 <= p <= n.
       s is the processor number, 0 <= s < p.
       If rev=true, the function assumes the processor numbering
       is bit reversed on input.
       rho_p is the bit-reversal permutation of length p.
    */
    const uint32_t np = n / p;
    const uint32_t ratio = c1 / c0;
    const uint32_t size = std::max( np / ratio, 1u );
    const uint32_t npackets = np / size;
    std::vector<double> tmp( 2 * size );

    uint32_t j0, j2;

    if ( rev )
    {
        j2 = rho_p[s] / c0;
        j0 = rho_p[s] - c0 * j2;
    }
    else
    {
        j2 = s / c0;
        j0 = s - c0 * j2;
    }

    const uint32_t jglobInit = j2 * c0 * np + j0;
    const uint32_t c1np = c1 * np;

    BSPProf::InitCommunication();

    for ( uint32_t j = 0; j < npackets; ++j )
    {
        //jglob = j2 * c0 * np + j * c0 + j0;
        const uint32_t jglob = jglobInit + j * c0;
        const uint32_t jglobDiv = jglob / c1np;

        //destproc = ( jglob / ( c1 * np ) ) * c1 + jglob % c1;
        const uint32_t destproc = jglobDiv * c1 + jglob % c1;

        //destindex = ( jglob % ( c1 * np ) ) / c1;
        const uint32_t destindex = ( jglob - jglobDiv * c1np ) / c1;

        for ( uint32_t r = 0; r < size; ++r )
        {
            const uint32_t r2 = r << 1;
            const uint32_t jRRatio = ( j + r * ratio ) << 1;
            tmp[r2] =   x[jRRatio];
            tmp[r2 + 1] = x[jRRatio + 1];
        }

        BSPLib::PutIterator( destproc, tmp.begin(), size * 2, x.begin(), destindex * 2 );
    }

    BSPProf::FinishCommunication();

    BSPLib::SyncPutRequests();
} /* end BSPRedistr */

void bspfft( std::vector<double> &x, uint32_t n, uint32_t p, uint32_t s, int sign, std::vector<double> &w0,
             std::vector<double> &w,
             std::vector<double> &tw, std::vector<uint32_t> &rho_np, std::vector<uint32_t> &rho_p )
{

    /* This parallel function computes the discrete Fourier transform
       of a complex array x of length n=2^m, m >= 1, stored in a real array
       of length 2n as pairs (Re x[j], Im x[j]), 0 <= j < n.
       x must have been registered before calling this function.
       p is the number of processors, p=2^q, 0 <= q < m.
       s is the processor number, 0 <= s < p.
       The function uses three weight tables:
           w0 for the unordered fft of length k1,
           w  for the unordered fft of length n/p,
           tw for a number of Twiddles, each of length n/p.
       The function uses two bit-reversal permutations:
           rho_np of length n/p,
           rho_p of length p.
       The weight tables and bit-reversal permutations must have been
       initialized before calling this function.
       If sign = 1, then the dft is computed,
           y[k] = sum j=0 to n-1 exp(-2*pi*i*k*j/n)*x[j], for 0 <= k < n.
       If sign =-1, then the inverse dft is computed,
           y[k] = (1/n) sum j=0 to n-1 exp(+2*pi*i*k*j/n)*x[j], for 0 <= k < n.
       Here, i=sqrt(-1). The output vector y overwrites x.
    */

    const uint32_t np = n / p;
    const uint32_t k1 = K1Init( n, p );
    Permute( x, np, rho_np );
    bool rev = true;

    for ( uint32_t r = 0, rEnd = np / k1; r < rEnd; ++r )
    {
        UFft( &x[2 * r * k1], k1, sign, w0 );
    }

    for ( uint32_t c = k1, c0 = 1, ntw = 0; c <= p; c *= np, ++ntw )
    {
        BSPRedistr( x, n, p, s, c0, c, rev, rho_p );
        rev = false;
        Twiddle( x, np, sign, &tw[2 * ntw * np] );
        UFft( x.data(), np, sign, w );
        c0 = c;
    }

    if ( sign == -1 )
    {
        const double ninv = 1 / ( double )n;

        for ( uint32_t j = 0; j < 2 * np; j++ )
        {
            x[j] *= ninv;
        }
    }

} /* end bspfft */

void BSPFftInit( uint32_t n, uint32_t p, uint32_t s, std::vector<double> &w0, std::vector<double> &w,
                 std::vector<double> &tw,
                 std::vector<uint32_t> &rho_np, std::vector<uint32_t> &rho_p )
{

    /* This parallel function initializes all the tables used in the FFT. */

    const uint32_t np = n / p;
    BitRevInit( np, rho_np );
    BitRevInit( p, rho_p );

    const uint32_t k1 = K1Init( n, p );
    UFftInit( k1, w0 );
    UFftInit( np, w );

    for ( uint32_t c = k1, ntw = 0; c <= p; c *= np, ++ntw )
    {
        const double alpha = ( s % c ) / ( double )( c );
        TwiddleInit( np, alpha, rho_np, &tw[2 * ntw * np] );
    }

} /* end BSPFftInit */
