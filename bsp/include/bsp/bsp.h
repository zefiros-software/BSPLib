/**
 * Copyright (c) 2015 Mick van Duijn, Koen Visscher and Paul Visscher
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
#ifndef __BSP_H__
#define __BSP_H__

#ifndef DEBUG
#   define BSP_SKIP_CHECKS
#endif

#include "bsp/bspExt.h"

#ifndef BSP_DISABLE_LEGACY

#define bsp_init BSP_NAMESPACE::Init
#define bsp_begin BSP_NAMESPACE::Begin
#define bsp_end BSP_NAMESPACE::End
#define bsp_pid BSP_NAMESPACE::ProcId
#define bsp_nprocs BSP_NAMESPACE::NProcs
#define bsp_abort BSP_NAMESPACE::Abort
#define bsp_vabort BSP_NAMESPACE::VAbort
#define bsp_sync BSP_NAMESPACE::Sync
#define bsp_time BSP_NAMESPACE::Time
#define bsp_push_reg BSP_NAMESPACE::PushReg
#define bsp_pop_reg BSP_NAMESPACE::PopReg
#define bsp_put BSP_NAMESPACE::Put
#define bsp_get BSP_NAMESPACE::Get
#define bsp_set_tagsize BSP_NAMESPACE::SetTagSize
#define bsp_send BSP_NAMESPACE::Send
#define bsp_hpsend BSP_NAMESPACE::HPSend
#define bsp_qsize BSP_NAMESPACE::QSize
#define bsp_get_tag BSP_NAMESPACE::GetTag
#define bsp_move BSP_NAMESPACE::Move
#define bsp_hpmove BSP_NAMESPACE::HPMove
#define bsp_hpput BSP_NAMESPACE::HPPut
#define bsp_hpget BSP_NAMESPACE::HPGet

#endif

#endif