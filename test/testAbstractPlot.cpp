#include "plot/abstractPlot.h"

#include "helper.h"

TEST( PX( AbstractPlot ), Construct )
{
    EXPECT_FALSE( std::is_constructible<AbstractPlot>::value );
}

TEST( PX( AbstractPlot ), GetBool )
{
    EXPECT_EQ( "True", AbstractPlot::GetBool( true ) );
    EXPECT_EQ( "False", AbstractPlot::GetBool( false ) );
}