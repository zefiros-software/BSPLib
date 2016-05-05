/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __EASYPLOT_H__
#define __EASYPLOT_H__

#include "plot/scatterPlot.h"
#include "plot/linePlot.h"
#include "plot/barPlot.h"
#include "plot/plot.h"
#include "plot/vec.h"

#include <valarray>
#include <vector>

namespace EasyPlot
{
    inline void ResidualPlot( ::Plot &plot, const Vec &exogenous, const Vec &endogenous, const Vec &fit )
    {
        plot.SubPlot( 1, 2, 1 )
        .AddPlot( LinePlot( exogenous, endogenous ) )
        .AddPlot( LinePlot( exogenous, fit ) )
        .SetXLabel( "X-Samples" )
        .SetYLabel( "Y-Value" )
        .SetTitle( "Residual Plot" )
        .SetLegend( { "Actual", "Fit" } )
        .SetXLimit( exogenous.Min(), exogenous.Max() );

        auto resid = endogenous.GetData();
        auto f = fit.GetData();

        for ( size_t i = 0, size = resid.size(); i < size; ++i )
        {
            resid[i] -= f[i];
        }

        plot.SubPlot( 1, 2, 2 )
        .AddPlot( ScatterPlot( exogenous, resid ) )
        .SetXLabel( "X-Samples" )
        .SetYLabel( "Residual" )
        .SetLegend( { "Residual" } )
        .SetXLimit( exogenous.Min(), exogenous.Max() );
    }

    template< typename tBackHueFunc, typename tForeHueFunc >
    inline void BackgroundForegroundBarPlot( ::Plot &plot, const std::vector< std::vector< double > > &background,
                                             const std::vector< std::vector< double > > &foreground, const std::vector< std::vector< double > > &x,
                                             const Palette &backPalette, const Palette &forePalette, const tBackHueFunc &backHue, const tForeHueFunc &foreHue )
    {
        std::vector< std::pair< Vec, Vec > > backData;
        std::vector< std::pair< Vec, Vec > > foreData;
        std::vector< size_t > hueData;

        for ( size_t i = 0, end = background.size(); i < end; ++i )
        {
            std::valarray< double > backVal( background[i].data(), background[i].size() );
            backVal += std::valarray< double >( foreground[i].data(), foreground[i].size() );

            backData.emplace_back( Vec( x[i] ), Vec( backVal ) );
            foreData.emplace_back( Vec( x[i] ), Vec( foreground[i] ) );
            hueData.push_back( i );
        }

        plot.AddPlot( BarPlot( backData, hueData, backHue ).SetColourMap( backPalette ).SetHatch( "//" ) );
        plot.AddPlot( BarPlot( foreData, hueData, foreHue ).SetColourMap( forePalette ) );
        plot.SetLegend( Plot::Location::Best );
    }
    
    template< typename tFunc >
    inline void StackedBarPlot( ::Plot &plot, const std::vector< std::vector< double > > &xValues, const std::vector< std::vector< double > > &yValues, const Palette &palette, const tFunc &hueFunc )
    {
        std::vector< Vec > yData;
        
        std::valarray< double > yValData( yValues[0].data(), yValues[0].size() );
        yData.emplace_back( Vec( yValues[0] ) );
        
        for( size_t i = 1, end = yValues.size(); i < end; ++i )
        {
            yValData += std::valarray< double >( yValues[i].data(), yValues[i].size() );
            yData.emplace_back( Vec( yValData ) );
        }
        
        std::vector< int32_t > hueData;
        
        plot.AddColourCycler( palette );
        
        for( int32_t i = ( int32_t )yValues.size() - 1; i >=0; --i )
        {
            plot.AddPlot( BarPlot( Vec( xValues[i] ), yData[i] ).UseColourCycler( plot.GetColourCycler() ) );
            hueData.push_back( i );
        }
        
        plot.AddCustomLegend( CustomLegend( palette, hueData, hueFunc ) );
    }
    
    template< typename tFunc >
    inline void StackedDistancedBarPlot( ::Plot &plot, const std::vector< std::vector< double > > &gapWidths, const std::vector< std::vector< double > > &barWidths, const std::vector< std::vector< double > > &yValues, const Palette &palette, const tFunc &hueFunc )
    {
        std::vector< Vec > yData;
        std::vector<double> lineX;
        std::vector<std::vector<double>> lineY( gapWidths.size() );
        
        lineX.push_back( 0 );
        
        for( auto &yVec : lineY )
        {
            yVec.push_back( 0 );
        }      
        
        std::valarray< double > yValData( yValues[0].data(), yValues[0].size() );
        yData.emplace_back( Vec( yValues[0] ) );
        
        for( auto height: yValData )
        {
            lineY[0].push_back( height );
            lineY[0].push_back( height );
        }
        
        for( size_t i = 1, end = yValues.size(); i < end; ++i )
        {
            yValData += std::valarray< double >( yValues[i].data(), yValues[i].size() );
        
            for( auto height: yValData )
            {
                lineY[i].push_back( height );
                lineY[i].push_back( height );
            }
            
            yData.emplace_back( Vec( yValData ) );
        }
        
        
        std::vector<double> x;
        std::vector<double> widths;
        
        {
            double left = 0;
            for( uint32_t j = 0; j < barWidths[0].size(); ++j )
            {
                double maxGap = gapWidths[0][j];
                double maxBar = barWidths[0][j];
                
                for( uint32_t i = 1; i < barWidths.size(); ++i )
                {
                    if( gapWidths[i][j] > maxGap )
                    {
                        maxGap = gapWidths[i][j];
                    }
                    
                    if( barWidths[i][j] > maxBar )
                    {
                        maxBar = barWidths[i][j];
                    }
                }
                
                left += maxGap;
                lineX.push_back( left );                                    
                x.push_back( left );
                widths.push_back( maxBar );
                
                left += maxBar;
                lineX.push_back( left );
            }
        }
        
        std::vector< int32_t > hueData;
        
        plot.AddColourCycler( palette );
        
        for( int32_t i = (int32_t)lineY.size() - 1; i >= 0; --i )
        {
            plot.AddPlot( LinePlot( lineX, lineY[i] ).SetColour( "black" ).SetWidth( 2.5 ).SetAlpha( 0.2 ).SetLineStyle( "--" ) );
            plot.AddPlot( LinePlot( lineX, lineY[i] ).UseColourCycler( plot.GetColourCycler() ).SetWidth(1).SetLineStyle( "--" ) );
        }
        
        plot.AddColourCycler( palette );
        
        for( int32_t i = ( int32_t )yValues.size() - 1; i >=0; --i )
        {
            plot.AddPlot( DistancedBarPlot( x, yData[i], widths ).UseColourCycler( plot.GetColourCycler() ) );
            hueData.push_back( i );
        }
        
        plot.AddCustomLegend( CustomLegend( palette, hueData, hueFunc ) );
    }
}

#endif