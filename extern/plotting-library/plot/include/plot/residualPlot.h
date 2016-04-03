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
#ifndef __RESIDUALPLOT_H__
#define __RESIDUALPLOT_H__

#include "plot/abstractPlot.h"

#include <string>

class ResidualPlot
    : public AbstractPlot
{
public:

    ResidualPlot( const Vec &exogenous, const Vec &endogenous )
    {
        mStream << "sns.residplot(" << ToArray( exogenous ) << "," << ToArray( endogenous );
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    ResidualPlot &SetLowess( bool lowess )
    {
        mStream << ", lowess=" << GetBool( lowess );
        return *this;
    }

    ResidualPlot &SetXPartial( const Mat &mat )
    {
        mStream << ", x_partial=" << ToArray( mat );
        return *this;
    }

    ResidualPlot &SetYPartial( const Mat &mat )
    {
        mStream << ", y_partial=" << ToArray( mat );
        return *this;
    }

    ResidualPlot &SetOrder( size_t order )
    {
        mStream << ", order=" << order;
        return *this;
    }

    ResidualPlot &SetRobust( bool robust )
    {
        mStream << ", robust=" << GetBool( robust );
        return *this;
    }

    ResidualPlot &SetLabel( const std::string &label )
    {
        mStream << ", label=" << label;
        return *this;
    }

    ResidualPlot &SetColour( const std::string &colour )
    {
        mStream << ", color='" << colour << "'";
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif