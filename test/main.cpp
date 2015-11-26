#define BSP_SKIP_CHECKS
#define BSP_THROW

#include "bsp/bsp.h"
#include "gtest/gtest.h"

#include "testClassic.h"

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );

    int result = RUN_ALL_TESTS();

#ifdef _WIN32
    system( "pause" );
#endif // _WIN32


    return result;
}