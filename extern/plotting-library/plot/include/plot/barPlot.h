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
#ifndef __BARPLOT_H__
#define __BARPLOT_H__

#include "plot/abstractPlot.h"
#include "plot/palette.h"

#include <string>
#include <tuple>

class BarPlot
    : public AbstractPlot
{
public:

    enum class Orientation
    {
        Vertical,
        Horizontal
    };

    BarPlot( const Vec &x, const Vec &y )
    {
        mStream << "sns.barplot(" << ToArray( x ) << "," << ToArray( y );
    }

    BarPlot( const std::vector< std::pair< Vec, Vec > > &data )
    {
        mStream << "x = []\ny = []\n";
        mStream << "x = x ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( tup.first );
        }

        mStream << "\ny = y ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( tup.second );
        }

        mStream << "\nsns.barplot( x, y";
    }

    BarPlot( const std::vector< std::pair< Vec, Vec > > &data, const std::vector< std::string > &hue )
    {
        mStream << "x = []\ny = []\nh = []\n";
        mStream << "x = x ";

        for ( auto & tup : data )
        {
            assert( tup.first.GetSize() == tup.second.GetSize() );
            mStream << "+ " << ToArray( tup.first );
        }

        mStream << "\ny = y ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( tup.second );
        }

        size_t i = 0;

        mStream << "\nh = h ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( std::vector< std::string >( tup.first.GetSize(), hue[i++] ) );
        }

        mStream << "\nsns.barplot( x, y, h";
    }

    template< typename tT, typename tFunc >
    BarPlot( const std::vector< std::pair< Vec, Vec > > &data, const std::vector< tT > &hueData, const tFunc &hueFunc )
    {
        std::vector< std::string > hue;

        for ( const auto & hueValue : hueData )
        {
            hue.emplace_back( hueFunc( hueValue ) );
        }

        InitData( data );
        InitHue( data, hue );
        PlotDataHue();
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    BarPlot &SetOrder( const Vec &order )
    {
        mStream << ", order = " << ToArray( order );
        return *this;
    }

    BarPlot &SetHueOrder( const std::vector< std::string > &order )
    {
        mStream << ", hue_order = " << ToArray( order );
        return *this;
    }

    BarPlot &SetConfidenceInterval( double ci )
    {
        mStream << ", ci = " << ci;
        return *this;
    }

    BarPlot &SetNBoot( size_t bootstrap )
    {
        mStream << ", n_boot = " << bootstrap;
        return *this;
    }

    BarPlot &SetOrientation( Orientation orientation )
    {
        mStream << ", orient = " << ( orientation == Orientation::Horizontal ? "'h'" : "'v'" );
        return *this;
    }

    BarPlot &SetColour( const std::string &colour )
    {
        mStream << ", color = '" << colour << "'";
        return *this;
    }
    
    BarPlot &UseColourCycler( const std::string &colourCycler )
    {
        mStream << ", color = next(" << colourCycler << ")";
        return *this;
    }

    BarPlot &SetColourMap( const Palette &pallet )
    {
        mStream << ", palette = " << pallet.ToString();
        return *this;
    }

    BarPlot &SetSaturation( double sat )
    {
        mStream << ", saturation = " << sat;
        return *this;
    }

    BarPlot &SetErrorColour( const std::string &colour )
    {
        mStream << ", errcolor = '" << colour << "'";
        return *this;
    }
    
    BarPlot &SetHatch( const std::string &hatch )
    {
        mStream << ", hatch = '" << hatch << "'";
        return *this;
    }

private:

    std::stringstream mStream;

    void InitData( const std::vector< std::pair< Vec, Vec > > &data )
    {
        mStream << "x = []\ny = []\n";
        mStream << "x = x ";

        for ( auto & tup : data )
        {
            assert( tup.first.GetSize() == tup.second.GetSize() );
            mStream << "+ " << ToArray( tup.first );
        }

        mStream << "\ny = y ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( tup.second );
        }
    }

    void InitHue( const std::vector< std::pair< Vec, Vec > > &data, const std::vector< std::string > &hue )
    {
        size_t i = 0;

        mStream << "\nh = []\n";
        mStream << "\nh = h ";

        for ( auto & tup : data )
        {
            mStream << "+ " << ToArray( std::vector< std::string >( tup.first.GetSize(), hue[i++] ) );
        }
    }

    void PlotData()
    {
        mStream << "\nsns.barplot( x, y";
    }

    void PlotDataHue()
    {
        PlotData();

        mStream << ", h";
    }
};

#endif