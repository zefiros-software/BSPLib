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
#ifndef __CLUSTERMAPPLOT_H__
#define __CLUSTERMAPPLOT_H__

#include "plot/abstractPlot.h"

#include <string>

class ClusterMapPlot
    : public AbstractPlot
{
public:

    enum class Linkage
    {
        Single,
        Complete,
        Average,
        Weighted,
        Centroid,
        Median,
        Ward
    };

    enum class Metric
    {
        Euclidian,
        Minkowski,
        CityBlock,
        SEuclidean,
        SQEuclidean,
        Cosine,
        Correlation,
        Hamming,
        Jaccard,
        Chebyshev,
        Canberra,
        Braycurtis,
        Mahalanobis,
        Yule,
        Mathcing,
        Dice,
        Kulsinski,
        RogersTanimoto,
        RusselRao,
        Sokalsneath,
        WMinkowski
    };

    ClusterMapPlot( const Mat &map )
    {
        mStream << "sns.clustermap(" << ToArray( map );
    }

    virtual std::string ToString() const override
    {
        return mStream.str() + " )";
    }

    ClusterMapPlot &SetMethod( Linkage linkage )
    {
        mStream << ", method=" << GetLinkage( linkage );
        return *this;
    }

    ClusterMapPlot &SetMetric( Metric metric )
    {
        mStream << ", metric=" << GetMetric( metric );
        return *this;
    }

    ClusterMapPlot &SetZScore( bool rows )
    {
        mStream << ", z_score=" << ( rows ? 0 : 1 );
        return *this;
    }

    ClusterMapPlot &SetStandardScale( bool rows )
    {
        mStream << ", standard_scale=" << ( rows ? 0 : 1 );
        return *this;
    }

    ClusterMapPlot &SetFigSize( const std::pair< size_t, size_t > &size )
    {
        mStream << ", figsize=(" << size.first << "," << size.second << ")";
        return *this;
    }

    ClusterMapPlot &SetRowCluster( bool cluster )
    {
        mStream << ", row_cluster=" << GetBool( cluster );
        return *this;
    }

    ClusterMapPlot &SetColumnCluster( bool cluster )
    {
        mStream << ", col_cluster=" << GetBool( cluster );
        return *this;
    }

    ClusterMapPlot &SetRowLinkage( const Vec &linkage )
    {
        mStream << ", row_linkage=" << ToArray( linkage );
        return *this;
    }

    ClusterMapPlot &SetColumnLinkage( const Vec &linkage )
    {
        mStream << ", col_linkage=" << ToArray( linkage );
        return *this;
    }

    ClusterMapPlot &SetMask( const std::vector< bool > &mask )
    {
        mStream << ", mask=" << ToArray( mask );
        return *this;
    }

private:

    std::stringstream mStream;

    static std::string GetLinkage( Linkage linkage )
    {
        switch ( linkage )
        {
        case Linkage::Single:
            return "'single'";

        case Linkage::Complete:
            return "'complete'";

        case Linkage::Average:
            return "'average'";
            break;

        case Linkage::Weighted:
            return "'weighted'";
            break;

        case Linkage::Centroid:
            return "'centroid'";
            break;

        case Linkage::Median:
            return "'median'";
            break;

        case Linkage::Ward:
            return "'ward'";
        }

        return "";
    }

    static std::string GetMetric( Metric metric )
    {
        switch ( metric )
        {
        case Metric::Euclidian:
            return "'euclidean'";

        case Metric::Minkowski:
            return "'minkowski'";

        case Metric::CityBlock:
            return "'cityblock'";

        case Metric::SEuclidean:
            return "'seuclidean'";

        case Metric::SQEuclidean:
            return "'sqeuclidean'";

        case Metric::Cosine:
            return "'cosine'";

        case Metric::Correlation:
            return "'correlation'";

        case Metric::Hamming:
            return "'hamming'";

        case Metric::Jaccard:
            return "'jaccard'";

        case Metric::Chebyshev:
            return "'chebyshev'";

        case Metric::Canberra:
            return "'canberra'";

        case Metric::Braycurtis:
            return "'braycurtis'";

        case Metric::Mahalanobis:
            return "'mahalanobis'";

        case Metric::Yule:
            return "'yule'";

        case Metric::Mathcing:
            return "'matching'";

        case Metric::Dice:
            return "'dice'";

        case Metric::Kulsinski:
            return "'kulsinski'";

        case Metric::RogersTanimoto:
            return "'rogerstanimoto'";

        case Metric::RusselRao:
            return "'russellrao'";

        case Metric::Sokalsneath:
            return "'sokalmichener'";

        case Metric::WMinkowski:
            return "'wminkowski'";
        }

        return "";
    }
};

#endif