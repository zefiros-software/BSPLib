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
#ifndef __SEMILOGX_H__
#define __SEMILOGX_H__

#include "plot/abstractPlot.h"

#include <string>

class SemiLogX
    : public AbstractPlot
{
public:

    SemiLogX( const Vec &exogenous, const Vec &endogenous )
    {
        mStream << "plt.semilogx(" << ToArray( exogenous ) << "," << ToArray( endogenous );
    }

    SemiLogX( const Vec &exogenous, const Vec &endogenous, const std::string &marker )
    {
        mStream << "plt.semilogx(" << ToArray( exogenous ) << "," << ToArray( endogenous ) << ",marker=" << marker;
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    SemiLogX &SetAlpha( double alpha )
    {
        mStream << ", alpha=" << alpha;
        return *this;
    }

    SemiLogX &SetScalar( double scalar )
    {
        mStream << ", s=" << scalar;
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif