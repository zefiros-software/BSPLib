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
#ifndef __BSPLIB_VOIDRECORDER_H__
#define __BSPLIB_VOIDRECORDER_H__

#include "bsp/processorData.h"

class VoidRecorder
{
public:

    void ResetResize( uint32_t /*size*/ )
    {
    }

    void PauseRecording( uint32_t /*pid*/ )
    {
    }

    void ResumeRecording( uint32_t /*pid*/ )
    {
    }

    void InitSyncTimer( uint32_t /*pid*/ )
    {
    }

    void RecordPreSync( uint32_t /*pid*/ )
    {
    }

    void RecordPostSync( uint32_t /*pid*/ )
    {
    }

    void InitCommunication( uint32_t /*pid*/ )
    {
    }

    void ManualInitCommunicaion( uint32_t /*pid*/ )
    {
    }

    void FinishCommunication( uint32_t /*pid*/ )
    {
    }

    void ManualFinishCommunicaion( uint32_t /*pid*/ )
    {
    }

    void MarkSuperstep( uint32_t /*pid*/ )
    {
    }

    void MarkSuperstep( uint32_t /*pid*/, uint32_t /*superstep*/ )
    {
    }

    void RecordProcessorsData( uint32_t /*pid*/, const std::vector<ProcessorData> &/*processorsData*/ )
    {
    }

    void PlotData()
    {
    }
};

#endif