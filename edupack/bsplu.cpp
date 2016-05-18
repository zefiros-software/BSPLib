#include "bspedupack.h"

#define EPS 1.0e-15

void bsp_broadcast( double *x, int n, int src, int s0, int stride, int p0,
                    int s, int phase )
{
    /* Broadcast the vector x of length n from processor src to
       processors s0+t*stride, 0 <= t < p0. Here n >= 0, p0 >= 1.
       The vector x must have been registered previously.
       Processors are numbered in one-dimensional fashion.
       s = local processor identity.
       phase= phase of two-phase broadcast (0 or 1)
       Only one phase is performed, without synchronization.
    */

    int b, t, t1, dest, nbytes;

    b = ( n % p0 == 0 ?  n / p0 : n / p0 + 1 ); /* block size */

    if ( phase == 0 && s == src )
    {
        for ( t = 0; t < p0; t++ )
        {
            dest = s0 + t * stride;
            nbytes = std::min( b, n - t * b ) * SZDBL;

            if ( nbytes > 0 )
            {
                bsp_put( dest, &x[t * b], x, t * b * SZDBL, nbytes );
            }
        }
    }

    if ( phase == 1 && s % stride == s0 % stride )
    {
        t = ( s - s0 ) / stride; /* s = s0+t*stride */

        if ( 0 <= t && t < p0 )
        {
            nbytes = std::min( b, n - t * b ) * SZDBL;

            if ( nbytes > 0 )
            {
                for ( t1 = 0; t1 < p0; t1++ )
                {
                    dest = s0 + t1 * stride;

                    if ( dest != src )
                    {
                        bsp_put( dest, &x[t * b], x, t * b * SZDBL, nbytes );
                    }
                }
            }
        }
    }

} /* end bsp_broadcast */

int nloc( int p, int s, int n )
{
    /* Compute number of local components of processor s for vector
       of length n distributed cyclically over p processors. */

    return ( n + p - s - 1 ) / p ;

} /* end nloc */

void bsplu( int M, int N, int s, int t, int n, int *pi, double **a )
{
    /* Compute LU decomposition of n by n matrix A with partial pivoting.
       Processors are numbered in two-dimensional fashion.
       Program text for P(s,t) = processor s+t*M,
       with 0 <= s < M and 0 <= t < N.
       A is distributed according to the M by N cyclic distribution.
    */

    int nloc( int p, int s, int n );
    double *pa, *uk, *lk, *Max;
    int nlr, nlc, k, i, j, r, *Imax;

    nlr =  nloc( M, s, n ); /* number of local rows */
    nlc =  nloc( N, t, n ); /* number of local columns */

    bsp_push_reg( &r, SZINT );

    if ( nlr > 0 )
    {
        pa = a[0];
    }
    else
    {
        pa = NULL;
    }

    bsp_push_reg( pa, nlr * nlc * SZDBL );
    bsp_push_reg( pi, nlr * SZINT );
    uk = vecallocd( nlc );
    bsp_push_reg( uk, nlc * SZDBL );
    lk = vecallocd( nlr );
    bsp_push_reg( lk, nlr * SZDBL );
    Max = vecallocd( M );
    bsp_push_reg( Max, M * SZDBL );
    Imax = vecalloci( M );
    bsp_push_reg( Imax, M * SZINT );

    /* Initialize permutation vector pi */
    if ( t == 0 )
    {
        for ( i = 0; i < nlr; i++ )
        {
            pi[i] = i * M + s;    /* global row index */
        }
    }

    bsp_sync();
    BSPProf::ResumeRecording();
    BSPProf::MarkSuperstep( 0 );

    for ( k = 0; k < n; k++ )
    {
        int kr, kr1, kc, kc1, imax  = 0, smax, s1, t1;
        double absmax, max, pivot;

        /* Initialise smax to non-existent index */
        smax = -1;

        /****** Superstep 0 ******/
        kr =  nloc( M, s, k ); /* first local row with global index >= k */
        kr1 = nloc( M, s, k + 1 );
        kc =  nloc( N, t, k );
        kc1 = nloc( N, t, k + 1 );

        if ( k % N == t ) /* k=kc*N+t */
        {
            /* Search for local absolute maximum in column k of A */
            absmax = 0.0;
            imax = -1;

            for ( i = kr; i < nlr; i++ )
            {
                if ( fabs( a[i][kc] ) > absmax )
                {
                    absmax = fabs( a[i][kc] );
                    imax = i;
                }
            }

            if ( absmax > 0.0 )
            {
                max = a[imax][kc];
            }
            else
            {
                max = 0.0;
            }

            /* Broadcast value and local index of maximum to P(*,t) */
            for ( s1 = 0; s1 < M; s1++ )
            {
                bsp_put( s1 + t * M, &max, Max, s * SZDBL, SZDBL );
                bsp_put( s1 + t * M, &imax, Imax, s * SZINT, SZINT );
            }
        }

        bsp_sync();
        BSPProf::MarkSuperstep();

        /****** Superstep 1 ******/
        if ( k % N == t )
        {
            /* Determine global absolute maximum (redundantly) */
            absmax = 0.0;

            for ( s1 = 0; s1 < M; s1++ )
            {
                if ( fabs( Max[s1] ) > absmax )
                {
                    absmax = fabs( Max[s1] );
                    smax = s1;
                }
            }

            if ( absmax > EPS )
            {
                r = Imax[smax] * M + smax; /* global index of pivot row */
                pivot = Max[smax];

                for ( i = kr; i < nlr; i++ )
                {
                    a[i][kc] /= pivot;
                }

                if ( s == smax )
                {
                    a[imax][kc] = pivot;    /* restore value of pivot */
                }

                /* Broadcast index of pivot row to P(*,*) */
                for ( t1 = 0; t1 < N; t1++ )
                {
                    bsp_put( s + t1 * M, &r, &r, 0, SZINT );
                }
            }
            else
            {
                bsp_abort( "bsplu at stage %d: matrix is singular\n", k );
            }
        }

        bsp_sync();
        BSPProf::MarkSuperstep();

        /****** Superstep 2 ******/
        if ( k % M == s )
        {
            /* Store pi(k) in pi(r) on P(r%M,0) */
            if ( t == 0 )
            {
                bsp_put( r % M, &pi[k / M], pi, ( r / M )*SZINT, SZINT );
            }

            /* Store row k of A in row r on P(r%M,t) */
            bsp_put( r % M + t * M, a[k / M], pa, ( r / M )*nlc * SZDBL, nlc * SZDBL );
        }

        if ( r % M == s )
        {
            if ( t == 0 )
            {
                bsp_put( k % M, &pi[r / M], pi, ( k / M )*SZINT, SZINT );
            }

            bsp_put( k % M + t * M, a[r / M], pa, ( k / M )*nlc * SZDBL, nlc * SZDBL );
        }

        bsp_sync();
        BSPProf::MarkSuperstep();

        /****** Superstep 3 ******/
        /* Phase 0 of two-phase broadcasts */
        if ( k % N == t )
        {
            /* Store new column k in lk */
            for ( i = kr1; i < nlr; i++ )
            {
                lk[i - kr1] = a[i][kc];
            }
        }

        if ( k % M == s )
        {
            /* Store new row k in uk */
            for ( j = kc1; j < nlc; j++ )
            {
                uk[j - kc1] = a[kr][j];
            }
        }

        bsp_broadcast( lk, nlr - kr1, s + ( k % N )*M,  s, M, N, s + t * M, 0 );
        bsp_broadcast( uk, nlc - kc1, ( k % M ) + t * M, t * M, 1, M, s + t * M, 0 );
        bsp_sync();

        BSPProf::MarkSuperstep();

        /****** Superstep 4 ******/
        /* Phase 1 of two-phase broadcasts */
        bsp_broadcast( lk, nlr - kr1, s + ( k % N )*M,  s, M, N, s + t * M, 1 );
        bsp_broadcast( uk, nlc - kc1, ( k % M ) + t * M, t * M, 1, M, s + t * M, 1 );
        bsp_sync();

        BSPProf::MarkSuperstep( 0 );

        /****** Superstep 0 ******/
        /* Update of A */
        for ( i = kr1; i < nlr; i++ )
        {
            for ( j = kc1; j < nlc; j++ )
            {
                a[i][j] -= lk[i - kr1] * uk[j - kc1];
            }
        }
    }

    BSPProf::PauseRecording();

    bsp_pop_reg( Imax );
    vecfreei( Imax );
    bsp_pop_reg( Max );
    vecfreed( Max );
    bsp_pop_reg( lk );
    vecfreed( lk );
    bsp_pop_reg( uk );
    vecfreed( uk );
    bsp_pop_reg( pi );
    bsp_pop_reg( pa );
    bsp_pop_reg( &r );

} /* end bsplu */
