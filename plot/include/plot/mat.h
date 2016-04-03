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
#ifndef __MAT_H__
#define __MAT_H__

#ifdef PLOTLIB_ARMA
#pragma warning(push)
#pragma warning(disable : 4702)
#include <armadillo>
#pragma warning(pop)
#endif

#include <initializer_list>
#include <algorithm>
#include <assert.h>
#include <vector>

class Mat
{
public:

    Mat( const std::vector< std::vector< double > > &data )
        : mData( data )
    {
        CheckDimensions( data );
    }

    Mat( const std::initializer_list< std::initializer_list< double > > &data )
        : mData( data.begin(), data.end() )
    {
        CheckDimensions( mData );
    }

    Mat( const std::vector< std::vector< std::string > > &data )
        : mStrData( data )
    {
    }

    Mat( const std::vector< std::vector< int64_t > > &data, const std::map< int64_t, std::string > &map )
    {
        mStrData.resize( data.size() );
        size_t i = 0;

        for ( auto &vec : data )
        {
            for ( auto &val : vec )
            {
                mStrData[i].push_back( map.at( val ) );
            }

            ++i;
        }
    }

#ifdef PLOTLIB_ARMA

    Mat( const arma::mat &data )
        : mData( data.n_rows )
    {
        size_t i = 0;

        data.each_row( [&]( const arma::rowvec & v )
        {
            mData[i++] = arma::conv_to< std::vector<double> >::from( v );
        } );
    }

#endif

    const std::vector< std::vector< double > > &GetData() const
    {
        return mData;
    }



    const std::vector< std::vector< std::string > > &GetStrings() const
    {
        return mStrData;
    }

    size_t GetSize() const
    {
        return mData.size();
    }

private:

    std::vector< std::vector< double > > mData;
    std::vector< std::vector< std::string > > mStrData;

    static void CheckDimensions( const std::vector< std::vector< double > > &data )
    {
        if ( !data.size() )
        {
            return;
        }

        const size_t size = ( *data.begin() ).size();

        for ( auto vec : data )
        {
            assert( vec.size() == size );
        }
    }

};

#endif