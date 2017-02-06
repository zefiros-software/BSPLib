/**
 * Copyright (c) 2017 Zefiros Software.
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
 */
#pragma once
#ifndef __BSPLIB_HISTORYRECORDER_H__
#define __BSPLIB_HISTORYRECORDER_H__

#include "bsp/processorData.h"

#ifdef BSP_USE_PROFILER
#include "plot/plotting.h"

enum class HistoryType : uint32_t
{
    None = 0x00,
    AnyData = 0x01,
    MatrixData = 0x02 | AnyData,
    BarData = 0x4 | AnyData,
    GroupedTimeBars = 0x08 | BarData,
    DistancedBars = 0x10 | BarData,
    MessageSize = 0x20 | AnyData,
    MessageCount = 0x40 | AnyData
};

constexpr HistoryType operator|( HistoryType a, HistoryType b )
{
    return static_cast<HistoryType>( static_cast<uint32_t>( a ) | static_cast<uint32_t>( b ) );
}

constexpr HistoryType operator&( HistoryType a, HistoryType b )
{
    return static_cast<HistoryType>( static_cast<uint32_t>( a ) & static_cast<uint32_t>( b ) );
}

constexpr HistoryType operator^( HistoryType a, HistoryType b )
{
    return static_cast<HistoryType>( static_cast<uint32_t>( a ) ^ static_cast<uint32_t>( b ) );
}

constexpr bool EitherOr( HistoryType reference, HistoryType a, HistoryType b )
{
    return ( ( reference & ( a | b ) ) ^ HistoryType::AnyData ) != HistoryType::None;
}

constexpr bool Contains( HistoryType reference, HistoryType a )
{
    return ( reference & a ) == a;
}

constexpr bool Both( HistoryType reference, HistoryType a, HistoryType b )
{
    return Contains( reference, a ) && Contains( reference, b );
}


template< HistoryType tHistoryType >
class HistoryRecorder
{
public:

    void ResetResize( uint32_t size )
    {
        mComputeTimes.clear();
        mComputeTimes.resize( size );

        mSyncTimes.clear();
        mSyncTimes.resize( size );

        mTimers.resize( size );

        mIndices.clear();
        mIndices.resize( size, 0 );

        mSuperstepIndices.clear();
        mSuperstepIndices.resize( size );

        mMaxSizes.clear();
        mMaxSizes.resize( size );

        mSuperstepIndices.clear();
        mSuperstepIndices.resize( size );

        mRecording.clear();
        mRecording.resize( size, true );

        mCommuniationTimes.clear();
        mCommuniationTimes.resize( size );

        mCommTimers.clear();
        mCommTimers.resize( size );

        mManualComm.clear();
        mManualComm.resize( size, false );

        mMaxCounts.clear();
        mMaxCounts.resize( size );

        mSendSizes.clear();
        mSendSizes.resize( size );
    }

    void PauseRecording( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            mRecording[pid] = false;
        } );
    }

    void ResumeRecording( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            mRecording[pid] = true;
            mTimers[pid].Tic();
        } );
    }

    void InitSyncTimer( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            mTimers[pid].Tic();
            mCommuniationTimes[pid] = 0;
        } );
    }

    void RecordPreSync( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            InitCommunication( pid );
        } );
    }

    void RecordPostSync( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            if ( mRecording[pid] )
            {
                FinishCommunication( pid );

                mSuperstepIndices[pid].push_back( mIndices[pid] );
                mSyncTimes[pid].push_back( mCommuniationTimes[pid] );
                mComputeTimes[pid].push_back( mTimers[pid].TocTic() - mCommuniationTimes[pid] );

                mCommuniationTimes[pid] = 0;
            }
        } );
    }

    void InitCommunication( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            if ( mRecording[pid] && !mManualComm[pid] )
            {
                mCommTimers[pid].Tic();
            }
        } );
    }

    void ManualInitCommunicaion( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            InitCommunication( pid );
            mManualComm[pid] = true;
        } );
    }

    void FinishCommunication( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            if ( mRecording[pid] & !mManualComm[pid] )
            {
                mCommuniationTimes[pid] += mCommTimers[pid].Toc();
            }
        } );
    }

    void ManualFinishCommunicaion( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            mManualComm[pid] = false;
            FinishCommunication( pid );
        } );
    }

    void MarkSuperstep( uint32_t pid )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [this, pid]
        {
            ++mIndices[pid];
        } );
    }

    void MarkSuperstep( uint32_t pid, uint32_t superstep )
    {
        BSPUtil::StaticIf< ( tHistoryType &HistoryType::AnyData ) == HistoryType::AnyData >( [&]
        {
            mIndices[pid] = superstep;
        } );
    }

    void RecordProcessorsData( uint32_t pid, const std::vector<ProcessorData> &processorsData )
    {
        constexpr bool barOrMatrixData = EitherOr( tHistoryType, HistoryType::BarData, HistoryType::MatrixData );
        constexpr bool sizeOrCountData = EitherOr( tHistoryType, HistoryType::MessageSize, HistoryType::MessageCount );
        BSPUtil::StaticIf < barOrMatrixData &&sizeOrCountData > ( [this, pid, &processorsData]
        {
            if ( mRecording[pid] )
            {
                size_t procs = processorsData.size();
                size_t sendBytes = 0;
                size_t receiveBytes = 0;
                size_t sendCount = 0;
                size_t receiveCount = 0;
                std::vector<double> sendSizes( procs, 0 );

                for ( uint32_t owner = 0; owner < procs; ++owner )
                {
                    const ProcessorData &data = processorsData[owner];

                    BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::BarData ) >( [&data, pid, owner, &receiveBytes, &receiveCount,
                    &sendCount]
                    {
                        BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageSize ) >( [&data, pid, &receiveBytes]
                        {
                            for ( auto it = data.putRequests[pid].CBegin(), end = data.putRequests[pid].CEnd(); it != end; ++it )
                            {
                                receiveBytes += it->size;
                            }


                            for ( auto it = data.tmpSendRequests[pid].CBegin(), end = data.tmpSendRequests[pid].CEnd(); it != end; ++it )
                            {
                                receiveBytes += it->bufferSize;
                                receiveBytes += it->tagSize;
                            }
                        } );

                        BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageCount ) >( [&data, pid, &receiveCount, &sendCount]
                        {
                            receiveCount += data.putRequests[pid].GetSize();
                            receiveCount += data.tmpSendRequests[pid].GetSize();
                            sendCount += data.getRequests[pid].GetSize();
                        } );
                    } );

                    BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageSize ) >( [&data, pid, owner, &sendBytes, &sendSizes]
                    {
                        BSPUtil::StaticIf< Both( tHistoryType, HistoryType::BarData, HistoryType::MatrixData ) >( [&data, pid, owner, &sendBytes, &sendSizes]
                        {
                            for ( auto it = data.getRequests[pid].CBegin(), end = data.getRequests[pid].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->size;
                                sendSizes[owner] += it->size;
                            }
                        } ).template
                        ElseIf< Contains( tHistoryType, HistoryType::BarData ) >( [&data, pid, &sendBytes]
                        {
                            for ( auto it = data.getRequests[pid].CBegin(), end = data.getRequests[pid].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->size;
                            }
                        } ).
                        Else( [&data, pid, owner, &sendSizes]
                        {
                            for ( auto it = data.getRequests[pid].CBegin(), end = data.getRequests[pid].CEnd(); it != end; ++it )
                            {
                                sendSizes[owner] += it->size;
                            }
                        } );
                    } );
                }

                const ProcessorData &data = processorsData[pid];

                for ( uint32_t target = 0; target < procs; ++target )
                {
                    BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageSize ) >( [&data, target, &sendBytes, &sendSizes]
                    {
                        BSPUtil::StaticIf< Both( tHistoryType, HistoryType::BarData, HistoryType::MatrixData ) >( [&data, target, &sendBytes, &sendSizes]
                        {
                            for ( auto it = data.putRequests[target].CBegin(), end = data.putRequests[target].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->size;
                                sendSizes[target] += it->size;
                            }

                            for ( auto it = data.tmpSendRequests[target].CBegin(), end = data.tmpSendRequests[target].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->bufferSize;
                                sendBytes += it->tagSize;
                                sendSizes[target] += it->bufferSize + it->tagSize;
                            }
                        } ).template
                        ElseIf< Contains( tHistoryType, HistoryType::BarData ) >( [&data, target, &sendBytes ]
                        {
                            for ( auto it = data.putRequests[target].CBegin(), end = data.putRequests[target].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->size;
                            }

                            for ( auto it = data.tmpSendRequests[target].CBegin(), end = data.tmpSendRequests[target].CEnd(); it != end; ++it )
                            {
                                sendBytes += it->bufferSize;
                                sendBytes += it->tagSize;
                            }
                        } ).
                        Else( [&data, target, &sendSizes]
                        {
                            for ( auto it = data.putRequests[target].CBegin(), end = data.putRequests[target].CEnd(); it != end; ++it )
                            {
                                sendSizes[target] += it->size;
                            }

                            for ( auto it = data.tmpSendRequests[target].CBegin(), end = data.tmpSendRequests[target].CEnd(); it != end; ++it )
                            {
                                sendSizes[target] += it->bufferSize + it->tagSize;
                            }
                        } );
                    } );

                    BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::BarData ) >( [&data, pid, target, &receiveBytes, &receiveCount,
                                                                                          &sendCount]
                    {
                        BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageSize ) >( [&data, target, &receiveBytes]
                        {
                            for ( auto it = data.getRequests[target].CBegin(), end = data.getRequests[target].CEnd(); it != end; ++it )
                            {
                                receiveBytes += it->size;
                            }
                        } );

                        BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::MessageCount ) >( [&data, pid, &sendCount, &receiveCount]
                        {
                            sendCount += data.putRequests[pid].GetSize();
                            sendCount += data.tmpSendRequests[pid].GetSize();
                            receiveCount += data.getRequests[pid].GetSize();
                        } );
                    } );
                }


                BSPUtil::StaticIf< Both( tHistoryType, HistoryType::MatrixData, HistoryType::MessageSize ) >( [this, pid, sendSizes]
                {
                    mSendSizes[pid].push_back( sendSizes );
                } );

                BSPUtil::StaticIf< Contains( tHistoryType, HistoryType::BarData ) >( [this, pid, sendBytes, receiveBytes, sendCount,
                                                                                      receiveCount]
                {
                    mMaxSizes[pid].push_back( std::max( sendBytes, receiveBytes ) * 1.0 );
                    mMaxCounts[pid].push_back( std::max( sendCount, receiveCount ) * 1.0 );
                } );
            }
        } );
    }

    void PlotData()
    {
        Plot plot;

        PlotSuperstepTimes( plot );
        plot.Figure();
        PlotGroupedBars( plot );
        plot.Figure();
        PlotWidthBars( plot );
        plot.Figure();
        PlotRatio( plot );
		//enabling this will result in an extra empty plot
        //plot.Figure();
        PlotMatrices( plot );
		plot.Show();
    }

private:

    std::vector<std::vector<double>> mSuperstepIndices;
    std::vector<std::vector<double>> mComputeTimes;
    std::vector<std::vector<double>> mSyncTimes;

    std::vector<std::vector<double>> mMaxSizes;
    std::vector<std::vector<std::vector<double>>> mSendSizes;

    std::vector<std::vector<double>> mMaxCounts;

    std::vector<BSPUtil::TicTimer> mTimers;
    std::vector<BSPUtil::TicTimer> mCommTimers;
    std::vector<uint32_t> mIndices;
    std::vector<double> mCommuniationTimes;

    std::vector<char> mRecording;
    std::vector<char> mManualComm;

    void PlotSuperstepTimes( Plot &plot )
    {
        Palette backPalette( Palette::Colour::Reds, mComputeTimes.size() );
        Palette forePalette( Palette::Colour::Blues, mComputeTimes.size() );

        auto backHue = []( size_t i )
        {
            return std::to_string( i ) + " Comm + Sync";
        };

        auto foreHue = []( size_t i )
        {
            return std::to_string( i ) + " Compute";
        };

        EasyPlot::BackgroundForegroundBarPlot( plot, ToPVecVec(mSyncTimes), ToPVecVec(mComputeTimes), ToPVecVec(mSuperstepIndices), backPalette, forePalette,
                                               backHue,
                                               foreHue );
        plot.SetXLabel( "Superstep (grouped)" ).SetYLabel( "Time (s)" );
        plot.SetTitle( "Computation, communication and synchronisation time, grouped by superstep number" );
        plot.SetSize( 900, 600 );
    }

    void PlotGroupedBars( Plot &plot )
    {
        CubeHelixPalette palette;
        palette.SetColours( mSuperstepIndices.size() ).SetGamma( 0.8 );

        auto hueFunc = []( int32_t i )
        {
            return "Proc " + std::to_string( i );
        };

        plot.SubPlot( 1, 2, 1 );
        EasyPlot::StackedBarPlot( plot, ToPVecVec(mSuperstepIndices), ToPVecVec(mMaxSizes), palette, hueFunc );
        plot.SetXLabel( "Superstep (grouped)" ).SetYLabel( "Bytes communicated = max(send, receive)" );

        plot.SubPlot( 1, 2, 2 );
        EasyPlot::StackedBarPlot( plot, ToPVecVec(mSuperstepIndices), ToPVecVec(mMaxCounts), palette, hueFunc );
        plot.SetXLabel( "Superstep (grouped)" ).SetYLabel( "Messages communicated = max(send, receive)" );

        plot.SetSupTitle( "#communications, grouped by superstep number" );
        plot.SetSize( 1050, 400 );
    }

    void PlotWidthBars( Plot &plot )
    {
        CubeHelixPalette palette;
        palette.SetColours( mSuperstepIndices.size() ).SetGamma( 0.8 );

        auto hueFunc = []( int32_t i )
        {
            return "Proc " + std::to_string( i );
        };

        plot.SubPlot( 1, 2, 1 );
        EasyPlot::StackedDistancedBarPlot( plot, ToPVecVec(mComputeTimes), ToPVecVec(mSyncTimes), ToPVecVec(mMaxSizes), palette, hueFunc );
        plot.SetXLabel( "Time (s)" ).SetYLabel( "Bytes communicated = max(send, receive)" );

        plot.SubPlot( 1, 2, 2 );
        EasyPlot::StackedDistancedBarPlot( plot, ToPVecVec(mComputeTimes), ToPVecVec(mSyncTimes), ToPVecVec(mMaxCounts), palette, hueFunc );
        plot.SetXLabel( "Time (s)" ).SetYLabel( "Messages communicated = max(send, receive)" );

        plot.SetSupTitle( "#communications, width represents time" );
        plot.SetSize( 1050, 400 );
    }

    void PlotRatio( Plot &plot )
    {
        std::vector<size_t> hueData;

        for ( size_t i = 0; i < mComputeTimes.size(); ++i )
        {
            const std::vector<double> &compTimes = mComputeTimes[i];
            const std::vector<double> &syncTimes = mSyncTimes[i];

            std::vector<double> ratio;

            for ( size_t j = 0; j < compTimes.size(); ++j )
            {
                if ( syncTimes[j] != 0 )
                {
                    ratio.push_back( compTimes[j] / syncTimes[j] );
                }
            }

            hueData.push_back( i );

            plot.AddPlot( LinePlot( ratio ) );
        }

        auto hueFunc = []( size_t i )
        {
            return "Proc " + std::to_string( i );
        };

        plot.SetLegend( hueData, hueFunc );
        plot.SetXLabel( "Superstep" ).SetYLabel( "comp/(comm + sync)" );
		plot.SetYScale(SymLogScale());
        plot.SetTitle( "Ratio comp/(comm + sync) per superstep" );
        plot.SetSize( 1050, 400 );
    }

    void PlotMatrices( Plot &plot )
    {
        size_t nSupersteps = mSendSizes[0].size();
        size_t rows = ( size_t )( sqrt( nSupersteps * 1.0 ) + 0.5 );
        size_t columns = ( nSupersteps + rows - 1 ) / rows;
        double maxElem = 0;
        double minElem = mSendSizes[0][0][0];

        SubPlots subPlots( rows, columns );

        for ( const auto &source : mSendSizes )
        {
            for ( const auto &superstep : source )
            {
                double max = *std::max_element( superstep.begin(), superstep.end() );
                double min = *std::min_element( superstep.begin(), superstep.end() );

                if ( max > maxElem )
                {
                    maxElem = max;
                }

                if ( min < minElem )
                {
                    minElem = min;
                }
            }
        }

        for ( uint32_t superstep = 0; superstep < nSupersteps; ++superstep )
        {
            std::vector<std::vector<double>> matrix;

            for ( auto it = mSendSizes.begin(); it != mSendSizes.end(); ++it )
            {
                matrix.push_back( it->at( superstep ) );
            }

            HeatMapPlot heatMap( matrix );

            heatMap.SetMinValue( 0 )
            .SetMaxValue( maxElem )
            .Square( true )
            .SetLineWidths( 0.1 );

            subPlots.AddHeatMapPlot( heatMap );
        }

        plot.AddPlot( subPlots );
        plot.SetSupTitle( "#communications from processor left to processor bottom" );
        plot.SetSize( 840, 600 );
    }

	static std::vector<PVec> ToPVecVec(const std::vector<std::vector<double>> &v)
    {
		std::vector<PVec> s;
		for (const auto &c : v)
		{
			s.emplace_back(PVec(c));
		}
		return s;
    }
};
#endif

#endif
