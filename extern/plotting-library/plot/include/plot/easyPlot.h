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
#ifndef __EASYPLOT_H__
#define __EASYPLOT_H__

#include "plot/scatterPlot.h"
#include "plot/linePlot.h"
#include "plot/plot.h"
#include "plot/vec.h"
#include "plot/plot.h"

namespace EasyPlot
{
    void ResidualPlot( ::Plot &plot, const Vec &exogenous, const Vec &endogenous, const Vec &fit )
    {
        plot.SubPlot( 1, 2, 1 )
        .AddPlot( LinePlot( exogenous, endogenous ) )
        .AddPlot( LinePlot( exogenous, fit ) )
        .SetXLabel( "X-Samples" )
        .SetYLabel( "Y-Value" )
        .SetTitle( "Residual Plot" )
        .SetLegend( { "Actual", "Fit" } )
        .SetXLimit( exogenous.Min(), exogenous.Max() );

        auto resid = endogenous.GetData();
        auto f = fit.GetData();

        for ( size_t i = 0, size = resid.size(); i < size; ++i )
        {
            resid[i] -= f[i];
        }

        plot.SubPlot( 1, 2, 2 )
        .AddPlot( ScatterPlot( exogenous, resid ) )
        .SetXLabel( "X-Samples" )
        .SetYLabel( "Residual" )
        .SetLegend( { "Residual" } )
        .SetXLimit( exogenous.Min(), exogenous.Max() );
    }
}

#endif