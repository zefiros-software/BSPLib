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
#ifndef __ARROWPLOT_H__
#define __ARROWPLOT_H__

#include "plot/abstractPlot.h"

#include <string>

class ArrowPlot
    : public AbstractPlot
{
public:

    enum class Shape
    {
        Full,
        Left,
        Right
    };

    ArrowPlot( double x, double y, double dx, double dy )
    {
        mStream << "plt.arrow(" << x << "," << y << "," << dx << "," << dy;
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    ArrowPlot &SetWidth( double width )
    {
        mStream << ", width=" << width;
        return *this;
    }

    ArrowPlot &SetLengthIncludesHead( bool includes )
    {
        mStream << ", length_includes_head=" << GetBool( includes );
        return *this;
    }

    ArrowPlot &SetHeadWidth( double width )
    {
        mStream << ", head_width=" << width;
        return *this;
    }

    ArrowPlot &SetHeadLength( double length )
    {
        mStream << ", head_length=" << length;
        return *this;
    }

    ArrowPlot &SetShape( Shape shape )
    {
        mStream << ", shape=" << GetShape( shape );
        return *this;
    }

    ArrowPlot &SetOverhang( double overhang )
    {
        mStream << ", overhang=" << overhang;
        return *this;
    }

    ArrowPlot &SetHeadStartsAtZero( bool starts )
    {
        mStream << ", head_starts_at_zero=" << GetBool( starts );
        return *this;
    }

private:

    std::stringstream mStream;

    std::string GetShape( Shape shape )
    {
        switch ( shape )
        {
        case Shape::Full:
            return "'full'";

        case Shape::Left:
            return "'left'";

        case Shape::Right:
            return "'right'";
        }

        return "''";
    }

};

#endif