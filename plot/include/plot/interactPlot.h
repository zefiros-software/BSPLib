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
#ifndef __INTERACTPLOT_H__
#define __INTERACTPLOT_H__

#include "plot/abstractPlot.h"
#include "plot/vec.h"

#include <string>
#include "palette.h"

class InteractPlot
    : public AbstractPlot
{
public:

    InteractPlot( const Vec &endogenous, const Vec &endogenous2, const Vec &exogenous )
    {
        mStream << "sns.interactplot(" << ToArray( endogenous ) << "," << ToArray( endogenous2 ) << "," << ToArray( exogenous );
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    InteractPlot &SetFilled( bool filled )
    {
        mStream << ", filled =" << GetBool( filled );
        return *this;
    }

    InteractPlot &SetColourMap( Palette pallet )
    {
        pallet.SetColourMap( true );
        mStream << ", cmap =" << pallet.ToString();
        return *this;
    }

    InteractPlot &SetColourBar( bool bar )
    {
        mStream << ", colorbar =" << GetBool( bar );
        return *this;
    }

    InteractPlot &SetLevels( size_t levelCount )
    {
        mStream << ", levels =" << levelCount;
        return *this;
    }

    InteractPlot &SetLevels( const Vec &levels )
    {
        mStream << ", levels =" << ToArray( levels );
        return *this;
    }

    InteractPlot &SetLogistic( bool logistic )
    {
        mStream << ", logistic =" << GetBool( logistic );
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif