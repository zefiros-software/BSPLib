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
#ifndef __COUNTPLOT_H__
#define __COUNTPLOT_H__

#include "plot/abstractPlot.h"
#include "plot/palette.h"

#include <string>

class CountPlot
    : public AbstractPlot
{
public:

    enum class Orientation
    {
        Vertical,
        Horizontal
    };

    CountPlot( const Vec &x )
    {
        mStream << "sns.countplot(x=" << ToArray( x );
    }

    CountPlot( const std::vector< Vec > &data )
    {
        mStream << "x = []\n";
        mStream << "x = x ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( tup );
        }

        mStream << "\nsns.countplot( x=x";
    }

    CountPlot( const std::vector< Vec > &data, const std::vector< std::string > &hue )
    {
        mStream << "x = []\nh = []\n";
        mStream << "x = x ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( tup );
        }

        size_t i = 0;

        mStream << "\nh = h ";

        for ( auto &tup : data )
        {
            mStream << "+ " << ToArray( std::vector< std::string >( tup.GetSize(), hue[i++] ) );
        }

        mStream << "\nsns.countplot( x=x, hue=h";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    CountPlot &SetOrder( const Vec &order )
    {
        mStream << ", order=" << ToArray( order );
        return *this;
    }

    CountPlot &SetHueOrder( const std::vector< std::string > &order )
    {
        mStream << ", hue_order=" << ToArray( order );
        return *this;
    }

    CountPlot &SetOrientation( Orientation orientation )
    {
        mStream << ", orient=" << ( orientation == Orientation::Horizontal ? "'h'" : "'v'" );
        return *this;
    }

    CountPlot &SetColour( const std::string &colour )
    {
        mStream << ", color='" << colour << "'";
        return *this;
    }

    CountPlot &SetColourMap( Palette pallet )
    {
        mStream << ", palette =" << pallet.ToString();
        return *this;
    }

    CountPlot &SetSaturation( double sat )
    {
        mStream << ", saturation=" << sat;
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif