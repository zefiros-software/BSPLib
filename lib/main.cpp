#include "bsp.h"
#include "edupack/bspedupack.h"

#include <iostream>
#include <sstream>

void BspTest( BSP &bsp )
{
    bsp.Begin( 20 );

    size_t s = bsp.PID();
    size_t nprocs = bsp.NProcs();

    size_t i = s;

    bsp.PushReg( &i, sizeof( uint32_t ) );

    bsp.Sync();

    //bsp.Get( ( s + 7 ) % nprocs, &i, 0, &i, sizeof( uint32_t ) );

    bsp.Send( ( uint32_t )( ( s + 7 ) % nprocs ), NULL, &i, sizeof( uint32_t ) );

    bsp.Sync();

    bsp.Move( &i, sizeof( uint32_t ) );

    std::stringstream ss;
    ss << gPID << " has " << i << "\n";

    std::cout << ss.str();

    bsp.PopReg( &i );

    bsp.End();
}

#define BSPTEST 1


#if BSPTEST == 0
int main( int argc, char **argv )
{
    BSP bsp;

    bsp.Init( [&bsp]()
    {
        BspTest( bsp );
    }, argc, argv );

    BspTest( bsp );

    system( "pause" );
}
#endif

#if BSPTEST == 1
int main( int argc, char **argv )
{
    bsp_begin( 4 );
    printf( "Hello BSP Worldwide from process %d of %d\n", ( int )bsp_pid(), ( int )bsp_nprocs() );
    bsp_end();
}
#endif

#if BSPTEST == 1
#endif