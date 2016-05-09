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
#ifndef __VEC_H__
#define __VEC_H__

#ifdef PLOTLIB_ARMA
#pragma warning(push)
#pragma warning(disable : 4702)
#include <armadillo>
#pragma warning(pop)
#endif

#include <initializer_list>
#include <functional>
#include <algorithm>
#include <stdint.h>
#include <valarray>
#include <vector>
#include <map>

class Vec
{
    friend class Matrix;
public:

    template< typename tT >
    Vec( const std::vector< tT > &data )
        : mData( data.begin(), data.end() )
    {
    }

    template< typename tContainer >
    Vec( const tContainer &data )
        : mData( std::begin( data ), std::end( data ) )
    {
    }

    Vec( const std::vector< std::string > &data )
        : mStrData( data )
    {
    }

    Vec( const std::vector< int64_t > &data, const std::map< int64_t, std::string > &map )
    {
        for ( auto &val : data )
        {
            mStrData.push_back( map.at( val ) );
        }
    }

    Vec( double min, double max, size_t n )
        : Vec( LinSpace( min, max, n ) )
    {
    }

    Vec( double min, double max, size_t n, std::function< double( double ) > function )
        : Vec( LinSpace( min, max, n ), function )
    {
    }

    Vec( const Vec &x, std::function< double( double ) > function )
    {
        for ( const auto &val : x.GetData() )
        {
            mData.push_back( function( val ) );
        }
    }

    Vec( const std::initializer_list< double > &data )
        : mData( data )
    {
    }

#ifdef PLOTLIB_ARMA

    Vec( const arma::vec &data )
        : mData( data.begin(), data.end() )
    {
    }

#endif

    const std::vector< double > &GetData() const
    {
        return mData;
    }

    const std::vector< std::string > &GetStrings() const
    {
        return mStrData;
    }

    size_t GetSize() const
    {
        return mStrData.empty() ? mData.size() : mStrData.size();
    }

    double Min() const
    {
        return *std::min_element( mData.begin(), mData.end() );
    }

    double Max() const
    {
        return *std::max_element( mData.begin(), mData.end() );
    }

private:

    std::vector< double > mData;
    std::vector< std::string > mStrData;

    std::vector<double> LinSpace( double start, double end, size_t num )
    {
        double delta = ( end - start ) / ( num - 1 );

        std::vector<double> linspaced( num );
        linspaced[0] = start;

        for ( size_t i = 1; i < num; ++i )
        {
            linspaced[i] = start + delta * static_cast<int>( i );
        }

        return linspaced;
    }
};

#endif