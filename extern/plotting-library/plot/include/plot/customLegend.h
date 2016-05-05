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
#ifndef __CUSTOMLEGEND_H__
#define __CUSTOMLEGEND_H__

#include "plot/palette.h"

#include <stdint.h>
#include <sstream>
#include <tuple>

class CustomLegend
{
public:

    CustomLegend( const Palette &palette, const std::vector<std::string> &labels )
    {
        mStream << "cl_cycler = itertools.cycle( " << palette.ToString() << " )\n";
        mStream << "cl_patches = [";

        uint32_t i = 1;

        for ( const std::string &label : labels )
        {
            mStream << "mpatches.Patch(facecolor=next(cl_cycler), label='" << label << "')";

            if ( i++ < labels.size() )
            {
                mStream << ",";
            }
        }

        mStream << "]\nplt.legend(handles=cl_patches)\n";
    }
    
    template< typename tT, typename tFunc >
    CustomLegend( const Palette &palette, const std::vector< tT > &hueData, const tFunc &hueFunc )
    {
        mStream << "cl_cycler = itertools.cycle( " << palette.ToString() << " )\n";
        mStream << "cl_patches = [";

        uint32_t i = 1;

        for ( const tT &hue : hueData )
        {
            mStream << "mpatches.Patch(facecolor=next(cl_cycler), label='" << hueFunc( hue ) << "')";

            if ( i++ < hueData.size() )
            {
                mStream << ",";
            }
        }

        mStream << "]\nplt.legend(handles=cl_patches)\n";
    }

    std::string ToString() const
    {
        return mStream.str();
    }

protected:

    std::stringstream mStream;
};

#endif