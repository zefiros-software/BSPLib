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
#ifndef __BSPLIB_BSPPROF_H__
#define __BSPLIB_BSPPROF_H__

#include "bsp/bspClass.h"
#include "bsp/util.h"

#include <cstdint>

namespace BSPProf
{
    inline void MarkSuperstep()
    {
        BSP::GetInstance().MarkSuperstep();
    }

    inline void MarkSuperstep( uint32_t superstep )
    {
        BSP::GetInstance().MarkSuperstep( superstep );
    }

    inline void PauseRecording()
    {
        BSP::GetInstance().PauseRecording();
    }

    inline void ResumeRecording()
    {
        BSP::GetInstance().ResumeRecording();
    }

    inline void InitCommunication()
    {
        BSP::GetInstance().InitCommunication();
    }

    inline void FinishCommunication()
    {
        BSP::GetInstance().FinishCommunication();
    }
}

#endif