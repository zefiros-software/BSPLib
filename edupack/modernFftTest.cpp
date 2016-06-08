#include "modernFft.h"

/*  This is a test program which uses bspfft to perform
    a Fast Fourier Transform and its inverse.

    The input vector is defined by x[j]=j+i, for 0 <= j < n.
    Here i= sqrt(-1).

    The output vector should equal the input vector,
    up to roundoff errors. Output is by triples (j, Re x[j], Im x[j]).
    Warning: don't rely on this test alone to check correctness.
    (After all, deleting the main loop will give similar results ;)

*/

#define NITERS 5   /* Perform NITERS forward and backward transforms.
                      A large NITERS helps to obtain accurate timings. */
#define NPRINT 10u  /* Print NPRINT values per processor */
#define MEGA 1000000.0

void BSPFftTest()
{
    uint32_t p = BSPLib::NProcs();
    uint32_t s = BSPLib::ProcId();

    std::vector<double> errors( p );

    uint32_t n;
    BSPLib::Push( n );
    BSPLib::PushContainer( errors );
    BSPProf::PauseRecording();
    BSPLib::Sync();
    BSPProf::MarkSuperstep();

    if ( s == 0 )
    {
        printf( "Please enter length n: \n" );

        /*#ifdef _WIN32
                scanf_s( "%d", &n );
        #else
                scanf( "%d", &n );
        #endif*/

        n = 4096 * 4;

        if ( n < 2 * p )
        {
            BSPLib::Classic::Abort( "Error in input: n < 2p" );
        }

        for ( uint32_t q = 1; q < p; ++q )
        {
            BSPLib::Put( q, n );
        }
    }

    BSPLib::Sync();
    BSPProf::MarkSuperstep();

    if ( s == 0 )
    {
        printf( "FFT of vector of length %d using %d processors\n", n, p );
        printf( "performing %d forward and %d backward transforms\n",
                NITERS, NITERS );
    }

    /* Allocate, register,  and initialize vectors */
    const uint32_t np = n / p;

    std::vector<double> x( 2 * np );
    BSPLib::PushContainer( x );

    const uint32_t k1 = K1Init( n, p );

    std::vector<double> w0( k1 );
    std::vector<double> w( np );
    std::vector<double> tw( 2 * np + p );

    std::vector<uint32_t> rho_np( np );
    std::vector<uint32_t> rho_p( p );

    for ( uint32_t j = 0; j < np; j++ )
    {
        const uint32_t j2 = j << 1;
        const uint32_t jglob = j * p + s;
        x[j2] = ( double )jglob;
        x[j2 + 1] = 1.0;
    }

    BSPLib::Sync();
    BSPProf::MarkSuperstep();
    BSPProf::ResumeRecording();
    BSPUtil::TicTimer timer = BSPLib::Tic();


    /* Initialize the weight and bit reversal tables */
    for ( uint32_t it = 0; it < NITERS; ++it )
    {
        BSPFftInit( n, p, s, w0, w, tw, rho_np, rho_p );
    }

    BSPLib::Sync();
    BSPProf::MarkSuperstep();
    double initTime = timer.TocTic();

    /* Perform the FFTs */
    for ( uint32_t it = 0; it < NITERS; ++it )
    {
        bspfft( x, n, p, s, 1, w0, w, tw, rho_np, rho_p );
        bspfft( x, n, p, s, -1, w0, w, tw, rho_np, rho_p );
    }

    BSPProf::MarkSuperstep();
    BSPLib::Sync();
    BSPProf::MarkSuperstep();
    BSPProf::PauseRecording();
    double fftTime = timer.Toc();

    /* Compute the accuracy */
    double max_error = 0.0;

    for ( uint32_t j = 0; j < np; ++j )
    {
        const uint32_t jglob = j * p + s;
        const uint32_t j2 = j << 1;
        const double error_re = fabs( x[j2] - ( double )jglob );
        const double error_im = fabs( x[j2 + 1] - 1.0 );
        const double error = sqrt( error_re * error_re + error_im * error_im );

        if ( error > max_error )
        {
            max_error = error;
        }
    }

    BSPLib::PutIterator( 0, &max_error, 1, errors.begin(), s );
    BSPLib::Sync();
    BSPProf::MarkSuperstep();

    if ( s == 0 )
    {
        max_error = *std::max_element( errors.begin(), errors.end() );
    }

    for ( uint32_t j = 0, jn = std::min( NPRINT, np ); j < jn; ++j )
    {
        const uint32_t jglob = j * p + s;
        const uint32_t j2 = j << 1;
        printf( "proc=%d j=%d Re= %f Im= %f \n", s, jglob, x[j2], x[j2 + 1] );
    }

    fflush( stdout );
    BSPLib::Sync();
    BSPProf::MarkSuperstep();

    if ( s == 0 )
    {
        printf( "Time per initialization = %lf sec \n", initTime / NITERS );
        const double ffttime = fftTime / ( 2.0 * NITERS );
        printf( "Time per FFT = %lf sec \n", ffttime );
        const double nflops = 5 * n * log( ( double )n ) / log( 2.0 ) + 2 * n;
        printf( "Computing rate in FFT = %lf Mflop/s \n", nflops / ( MEGA * ffttime ) );
        printf( "Absolute error= %e \n", max_error );
        printf( "Relative error= %e \n\n", max_error / n );
    }

    BSPLib::PopContainer( x );
    BSPLib::PopContainer( errors );
    BSPLib::Pop( n );

    BSPLib::Sync();

} /* end bspfft_test */

int main( int /*argc*/, char ** /*argv*/ )
{
    printf( "How many processors do you want to use?\n" );
    fflush( stdout );

    uint32_t P;
    /*#ifdef _WIN32
        scanf_s( "%d", &P );
    #else
        scanf( "%d", &P );
    #endif*/

    P = 4;

    if ( P > BSPLib::NProcs() )
    {
        printf( "Sorry, not enough processors available.\n" );
        fflush( stdout );
        exit( 1 );
    }

    BSPLib::Execute( BSPFftTest, P );

    exit( 0 );

} /* end main */
