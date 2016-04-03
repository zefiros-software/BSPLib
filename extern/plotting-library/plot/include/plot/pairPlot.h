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
#ifndef __PAIRPLOT_H__
#define __PAIRPLOT_H__

#include "plot/abstractPlot.h"
#include "plot/mat.h"

#include <string>
#include "palette.h"

class PairPlot
    : public AbstractPlot
{
public:

    enum class Type
    {
        Scatter,
        Regression
    };

    enum class DiagonalType
    {
        Histogram,
        KernelDensity
    };

    PairPlot( const Mat &mat, const std::vector< std::string > &names )
    {
        assert( names.size() == mat.GetData().size() );
        mStream << "data = pd.DataFrame()\n";
        size_t i = 0;

        for ( const auto &column : mat.GetData() )
        {
            mStream << "data['" << names[i++] << "'] = " << ToArray( column ) << "\n";
        }

        mStream << "sns.pairplot( data";
    }

    PairPlot( const std::vector< Mat > &mats, const std::vector< std::string > &names,
              const std::vector< std::string > &hue )
    {
        assert( hue.size() == mats.size() );
        mStream << "data = pd.DataFrame()\nhue = []\n";

        size_t k = 0;

        for ( const auto &name : names )
        {
            mStream << "x" << k++ << " = []\n";
        }

        size_t j = 0;

        for ( const auto &mat : mats )
        {
            assert( names.size() == mat.GetData().size() );

            size_t i = 0;

            for ( const auto &column : mat.GetData() )
            {
                mStream << "x" << i << " = x" << i << " + " << ToArray( column ) << "\n";
                ++i;
            }

            mStream << "hue = hue + "
                    << ToArray( std::vector< std::string >( mat.GetData()[0].size(), hue[j++] ) ) << "\n";
        }

        size_t l = 0;

        for ( const auto &name : names )
        {
            mStream << "data['" << name << "']" << " = x" << l << "\n";
            ++l;
        }

        mStream << "data['hue'] = hue\n";
        mStream << "sns.pairplot( data, hue = 'hue'";
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    PairPlot &SetXVars( const std::vector< std::string > &xvars )
    {
        mStream << ", x_vars = " << ToArray( xvars );
        return *this;
    }

    PairPlot &SetYVars( const std::vector< std::string > &yvars )
    {
        mStream << ", y_vars = " << ToArray( yvars );
        return *this;
    }

    PairPlot &SetType( Type type )
    {
        mStream << ", kind = " << ( type == Type::Scatter ? "'scatter'" : "'reg'" );
        return *this;
    }

    PairPlot &SetDiagonalType( DiagonalType type )
    {
        mStream << ", diag_kind = " << ( type == DiagonalType::Histogram ? "'hist'" : "'kde'" );
        return *this;
    }

    PairPlot &SetMarker( const std::string &marker )
    {
        mStream << ", markers = '" << marker << "'";
        return *this;
    }

    PairPlot &SetMarkers( const std::vector<std::string> &marker )
    {
        mStream << ", markers = " << ToArray( marker );
        return *this;
    }

    PairPlot &SetSize( double size )
    {
        mStream << ", size = " << size;
        return *this;
    }

    PairPlot &SetAspect( double aspect )
    {
        mStream << ", aspect = " << aspect;
        return *this;
    }

private:

    std::stringstream mStream;
};

#endif