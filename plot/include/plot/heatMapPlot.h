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
#ifndef __HEATMAPPLOT_H__
#define __HEATMAPPLOT_H__


#include "plot/abstractPlot.h"

#include <string>

class SubPlots;

class HeatMapPlot
    : public AbstractPlot
{
public:

    friend class SubPlots;

    HeatMapPlot( const Mat &map )
    {
        mStream << "sns.heatmap(" << ToArray( map );
    }

    HeatMapPlot( const Vec &x, const Vec &y, size_t bins = 50 )
    {
        mStream << "heatmap, xedges, yedges = np.histogram2d( "
                << ToArray( x ) << ", " << ToArray( y ) << ", bins=" << bins << ")\n";
        mStream << "sns.heatmap(heatmap";
    }

    HeatMapPlot( const Vec &x, const Vec &y, std::pair< size_t, size_t > bins )
    {
        mStream << "heatmap, xedges, yedges = np.histogram2d( "
                << ToArray( x ) << ", " << ToArray( y ) << ", bins=(" << bins.first << "," << bins.second << "))\n";
        mStream << "sns.heatmap(heatmap";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    HeatMapPlot &SetMinValue( double value )
    {
        mStream << ", vmin = " << value;
        return *this;
    }

    HeatMapPlot &SetMaxValue( double value )
    {
        mStream << ", vmax = " << value;
        return *this;
    }

    HeatMapPlot &SetCenter( double value )
    {
        mStream << ", center = " << value;
        return *this;
    }

    HeatMapPlot &SetRobust( bool robust )
    {
        mStream << ", robust = " << GetBool( robust );
        return *this;
    }

    HeatMapPlot &SetAnnotate( bool annotate )
    {
        mStream << ", annot = " << GetBool( annotate );
        return *this;
    }

    HeatMapPlot &SetFormat( const std::string &fmt )
    {
        mStream << ", fmt = " << fmt;
        return *this;
    }

    HeatMapPlot &SetLineWidths( double value )
    {
        mStream << ", linewidths = " << value;
        return *this;
    }

    HeatMapPlot &SetColour( const std::string &colour )
    {
        mStream << ", color = '" << colour << "'";
        return *this;
    }

    HeatMapPlot &SetColourMap( Palette pallet )
    {
        pallet.SetColourMap( true );
        mStream << ", cmap = " << pallet.ToString();
        return *this;
    }

    HeatMapPlot &SetSquare( bool square )
    {
        mStream << ", square = " << GetBool( square );
        return *this;
    }

    HeatMapPlot &SetXTickLabel( const std::vector< std::string > &names )
    {
        mStream << ", xticklabels = '" << ToArray( names ) << "'";
        return *this;
    }

    HeatMapPlot &SetYTickLabel( const std::vector< std::string > &names )
    {
        mStream << ", yticklabels = '" << ToArray( names ) << "'";
        return *this;
    }

    HeatMapPlot &SetXTickLabel( bool enable )
    {
        mStream << ", xticklabels = " << GetBool( enable );
        return *this;
    }

    HeatMapPlot &SetYTickLabel( bool enable )
    {
        mStream << ", yticklabels = " << GetBool( enable );
        return *this;
    }

    HeatMapPlot &SetMask( const std::vector< bool > &mask )
    {
        mStream << ", mask = " << ToArray( mask );
        return *this;
    }
    
    HeatMapPlot &SetColourBar( bool enable )
    {
        mStream << ", cbar = " << GetBool( enable );
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif