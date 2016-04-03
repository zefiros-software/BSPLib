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
#ifndef __STRIPPLOT_H__
#define __STRIPPLOT_H__

#include "plot/abstractPlot.h"
#include "plot/palette.h"

#include <string>
#include <tuple>

class StripPlot
    : public AbstractPlot
{
public:

    enum class Orientation
    {
        Vertical,
        Horizontal
    };

    StripPlot( const Vec &x, const Vec &y )
    {
        mStream << "sns.stripplot(" << ToArray( x ) << "," << ToArray( y );
    }

    StripPlot( const std::vector< std::pair< Vec, Vec > > &data )
    {
        mStream << "x = []\ny = []\n";
        mStream << "x = x ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( tup.first );
        }

        mStream << "\ny = y ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( tup.second );
        }

        mStream << "\nsns.stripplot( x, y";
    }

    StripPlot( const std::vector< std::pair< Vec, Vec > > &data, const std::vector< std::string > &hue )
    {
        mStream << "x = []\ny = []\nh = []\n";
        mStream << "x = x ";

        for ( auto &tup : data )
        {
            assert( tup.first.GetSize() == tup.second.GetSize() );
            mStream << "+ " << ToArray( tup.first );
        }

        mStream << "\ny = y ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( tup.second );
        }

        size_t i = 0;

        mStream << "\nh = h ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( std::vector< std::string >( tup.first.GetSize(), hue[i++] ) );
        }

        mStream << "\nsns.stripplot( x, y, h";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    StripPlot &SetOrder( const Vec &order )
    {
        mStream << ", order=" << ToArray( order );
        return *this;
    }

    StripPlot &SetHueOrder( const std::vector< std::string > &order )
    {
        mStream << ", hue_order=" << ToArray( order );
        return *this;
    }

    StripPlot &SetJitter( double jitter )
    {
        mStream << ", jitter= " << jitter;
        return *this;
    }

    StripPlot &SetJitter( bool jitter )
    {
        mStream << ", jitter= " << GetBool( jitter );
        return *this;
    }

    StripPlot &SetSplit( bool split )
    {
        mStream << ", jitter= " << GetBool( split );
        return *this;
    }

    StripPlot &SetOrientation( Orientation orientation )
    {
        mStream << ", orient = " << ( orientation == Orientation::Horizontal ? "'h'" : "'v'" );
        return *this;
    }

    StripPlot &SetColour( const std::string &colour )
    {
        mStream << ", color = '" << colour << "'";
        return *this;
    }

    StripPlot &SetColourMap( Palette pallet )
    {
        mStream << ", palette = " << pallet.ToString();
        return *this;
    }

    StripPlot &SetScale( double scale )
    {
        mStream << ", scale=" << scale;
        return *this;
    }

    StripPlot &SetEdgdeColour( const std::string &colour )
    {
        mStream << ", edgecolor = '" << colour << "'";
        return *this;
    }

    StripPlot &SetLineWidth( double width )
    {
        mStream << ", linewidth=" << width;
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif