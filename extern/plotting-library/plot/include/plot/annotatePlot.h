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
#ifndef __ANNOTATEPLOT_H__
#define __ANNOTATEPLOT_H__

#include "plot/abstractPlot.h"

#include <string>

class AnnotatePlot
    : public AbstractPlot
{
public:

    enum class Type
    {
        FigurePoints,
        FigurePixels,
        FigureFraction,
        AxesPoints,
        AxesPixels,
        AxesFraction,
        Data,
        OffsetPoints,
        Polar
    };

    AnnotatePlot()
    {
        mStream << "plt.annotate(";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    AnnotatePlot &SetLabel( const std::string &label )
    {
        mStream << ", s=" << label;
        return *this;
    }

    AnnotatePlot &SetXY( double x, double y )
    {
        mStream << ", xy=(" << x << "," << y << ")";
        return *this;
    }

    AnnotatePlot &SetXYText( double x, double y )
    {
        mStream << ", xytext=(" << x << "," << y << ")";
        return *this;
    }

    AnnotatePlot &SetXYCoordinates( Type type )
    {
        mStream << ", xycoords=" << GetType( type );
        return *this;
    }

    AnnotatePlot &SetTextCoordinates( Type type )
    {
        mStream << ", textcoords=" << GetType( type );
        return *this;
    }

private:

    std::stringstream mStream;

    std::string GetType( Type type )
    {
        switch ( type )
        {
        case Type::FigurePoints:
            return "'figure points'";

        case Type::FigurePixels:
            return "'figure pixels'";

        case Type::FigureFraction:
            return "'figure fraction'";

        case Type::AxesPoints:
            return "'axes points'";

        case Type::AxesPixels:
            return "'axes pixels'";

        case Type::AxesFraction:
            return "'axes fraction'";

        case Type::Data:
            return "'data'";

        case Type::OffsetPoints:
            return "'offset points'";

        case Type::Polar:
            return "'polar'";
        }

        return "''";
    }

};

#endif