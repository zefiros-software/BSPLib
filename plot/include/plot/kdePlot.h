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
#ifndef __KDEPLOT_H__
#define __KDEPLOT_H__

#include "plot/abstractPlot.h"

#include <string>

class KDEPlot
    : public AbstractPlot
{
public:

    enum class Kernel
    {
        Gauss,
        Cos,
        Biw,
        Epa,
        Tri,
        Triw
    };

    enum class BW
    {
        Scott,
        Silverman
    };

    KDEPlot( const Vec &data )
    {
        mStream << "sns.kdeplot(np.array(" << ToArray( data ) << ")";
    }

    KDEPlot( const Vec &data, const Vec &data2 )
    {
        mStream << "sns.kdeplot( np.array(" << ToArray( data ) << "), np.array(" << ToArray( data2 ) << ")";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    KDEPlot &SetVertical( bool vertical )
    {
        mStream << ", kind = " << GetBool( vertical );
        return *this;
    }

    KDEPlot &SetKernel( Kernel kernel )
    {
        mStream << ", kernel = '" << GetKernel( kernel ) << "'";
        return *this;
    }

    KDEPlot &SetBW( BW bw )
    {
        mStream << ", bw = '" << GetBW( bw ) << "'";
        return *this;
    }

    KDEPlot &SetBW( double scalar )
    {
        mStream << ", bw = " << scalar;
        return *this;
    }

    KDEPlot &SetBW( const std::pair<double, double> &scalars )
    {
        mStream << ", bw = ( " << scalars.first << ", " << scalars.second << " )";
        return *this;
    }

    KDEPlot &SetGridSize( size_t size )
    {
        mStream << ", gridsize = " << size;
        return *this;
    }

    KDEPlot &SetCut( double cut )
    {
        mStream << ", cut = " << cut;
        return *this;
    }

    KDEPlot &SetClip( const std::pair<double, double> &clip )
    {
        mStream << ", ylim = ( " << clip.first << ", " << clip.second << " )";
        return *this;
    }

    KDEPlot &SetClip( const std::pair<double, double> &clipx, const std::pair<double, double> &clipy )
    {
        mStream << ", ylim = ( ( " << clipx.first << ", " << clipx.second << " ), ( "
                << clipy.first << ", " << clipy.second << " ) )";
        return *this;
    }

    KDEPlot &SetLegend( bool legend )
    {
        mStream << ", legend = " << GetBool( legend );
        return *this;
    }

    KDEPlot &SetCumulative( bool cumulative )
    {
        mStream << ", cumulative = " << GetBool( cumulative );
        return *this;
    }

    KDEPlot &SetShadeLowest( bool shadwLowest )
    {
        mStream << ", shade_lowest = " << GetBool( shadwLowest );
        return *this;
    }

private:

    std::stringstream mStream;

    static std::string GetBW( BW bw )
    {
        switch ( bw )
        {
        case BW::Scott:
            return "scott";

        case BW::Silverman:
            return "silverman";
        }

        return "";
    }

    static std::string GetKernel( Kernel kernel )
    {
        switch ( kernel )
        {
        case Kernel::Gauss:
            return "gau";

        case Kernel::Cos:
            return "cos";

        case Kernel::Biw:
            return "biw";

        case Kernel::Epa:
            return "epa";

        case Kernel::Tri:
            return "tri";

        case Kernel::Triw:
            return "triw";
        }

        return "";
    }
};

#endif