#include "bench.h"

#define PLOTLIB_ARMA
#include "plot/plotting.h"

template< typename tFunc >
void BenchCommunication( const tFunc &measureCommunication, uint32_t P, Plot &plot )
{
    std::vector< double > t( MAXH + 1 );
    double  r, g, l, g0, l0;

    BSPLib::Execute( [ &t, &r, &g0, &l0, &g, &l, &measureCommunication ]
    {
        uint32_t p = BSPLib::NProcs();
        uint32_t s = BSPLib::ProcId();

        std::vector< double > Time( p );
        std::vector< double > dest( 2 * MAXH + p );
        BSPLib::PushContainer( Time );
        BSPLib::PushContainer( dest );

        BSPLib::Sync();

        DetermineR( r, Time, s );

        DetermineGL( r, t, dest, Time, p, s, measureCommunication );

        if ( s == 0 )
        {
            ReportResults( g0, l0, p, r, g, l, t );
        }

        BSPLib::PopContainer( dest );
        BSPLib::PopContainer( Time );
    }, P );

    LinePlot line( PVec( 0, MAXH, MAXH, [g, l, r]( double h )
    {
        return ( g * h + l ) / r * 1000000;
    } ) );

    plot.AddPlot( LinePlot( vec( vec( t.data(), t.size(), false ) / r * 1000000 ) ) );
    plot.AddPlot( line );
}

namespace BenchType
{
    enum BenchType
    {
        BenchPut = 0x1,
        BenchGet = 0x2,
        BenchSend = 0x4
    };
}

void BSPBenchModern( uint32_t P, BenchType::BenchType benchType )
{
    Plot plot;

    if ( benchType & BenchType::BenchPut )
    {
        BenchCommunication( MeasurePut, P, plot );
    }

    if ( benchType & BenchType::BenchGet )
    {
        BenchCommunication( MeasureGet, P, plot );
    }

    if ( benchType & BenchType::BenchSend )
    {
        BenchCommunication( MeasureSend, P, plot );
    }

    plot.SetTightLayout().Show();
}

int main( int /*argc*/, char ** /*argv*/ )
{
#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc =  0;
#endif
    printf( "How many processors do you want to use?\n" );
    fflush( stdout );

    uint32_t P;

    //scanf_s( "%d", &P );
    P = 4;

    if ( P > bsp_nprocs() )
    {
        printf( "Sorry, not enough processors available.\n" );
        exit( 1 );
    }

    BSPBenchModern( P, BenchType::BenchPut );
}