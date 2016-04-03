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
#ifndef __ABSTRACTPLOT_H__
#define __ABSTRACTPLOT_H__

#include "plot/vec.h"
#include "plot/mat.h"

#include <sstream>
#include <string>
#include <vector>

class AbstractPlot
{
public:

    virtual ~AbstractPlot()
    {
    }

    virtual std::string ToString() const = 0;

    static std::string GetBool( bool boolean )
    {
        return boolean ? "True" : "False";
    }

    static std::string ToArray( const Vec &vec )
    {
        const std::vector< std::string > vecStr = vec.GetStrings();
        return vecStr.size() ? ToArray( vecStr ) : ToArray( vec.GetData() );
    }

    static std::string ToArray( const Mat &mat )
    {
        const std::vector<std::vector< std::string > > matStr = mat.GetStrings();
        return matStr.size() ? ToArray( matStr ) : ToArray( mat.GetData() );
    }

    static std::string ToArray( const std::vector< std::vector< double > > &mat )
    {
        std::stringstream stream;
        stream << "[";
        bool first = true;

        for ( auto vec : mat )
        {
            if ( !first )
            {
                stream << ",";
            }

            stream << "[" + ToString( vec.begin(), vec.end() ) + "]";
            first = false;
        }

        return stream.str() + "]";
    }

    static std::string ToArray( const std::vector< std::vector< std::string > > &mat )
    {
        std::stringstream stream;
        stream << "[";
        bool first = true;

        for ( auto vec : mat )
        {
            if ( !first )
            {
                stream << ",";
            }

            stream << "[" + ToString( vec.begin(), vec.end() ) + "]";
            first = false;
        }

        return stream.str() + "]";
    }

    static std::string ToArray( const std::vector< double > &vec )
    {
        return "[" + ToString( vec.begin(), vec.end() ) + "]";
    }

    static std::string ToArray( const std::vector<std::string> &vec )
    {
        return "[" + ToString( vec.begin(), vec.end(), "'" ) + "]";
    }

    static std::string ToArray( const std::vector<bool> &vec )
    {
        return "[" + ToString( vec.begin(), vec.end() ) + "]";
    }

protected:

    template<typename tT>
    static std::string ToString( tT begin, tT end, const std::string &delimiter = "" )
    {
        std::stringstream ss;
        bool first = true;

        for ( ; begin != end; ++begin )
        {
            if ( !first )
            {
                ss << ", ";
            }

            ss << delimiter << *begin << delimiter;
            first = false;
        }

        return ss.str();
    }
};

#endif