#include "helper.h"

TEST( PX( DirectOLS ), Simple )
{
    vec ex = linspace( 0, 1, 1000 );
    vec distr = randn( 1000 );
    vec distr2 = randn( 1000 );
    mat ran = randn( 3, 200 );
    mat ran2 = randn( 3, 200 );
    Plot()
    .AddPlot( FactorBarPlot( {{3, 4, 5}, {1, 2, 3} } ) )
    .SetXLabel( "LOL" )
    .SetLegend( {"X", "Y"} )
    .Show();
}