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
#ifndef __BSPLIB_BSP_H__
#define __BSPLIB_BSP_H__

#ifndef DEBUG
#   define BSP_SKIP_CHECKS
#endif

#include "bsp/bspExt.h"

#ifndef BSP_DISABLE_NAMESPACE
#   define BSP_FULL_NAMESPACE BSP_NAMESPACE::Classic
#else
#   define BSP_FULL_NAMESPACE ::Classic
#endif

#ifndef BSP_DISABLE_LEGACY

/// Provides and overload to the old legacy interface

#define bsp_init BSP_FULL_NAMESPACE::Init
#define bsp_begin BSP_FULL_NAMESPACE::Begin
#define bsp_end BSP_FULL_NAMESPACE::End
#define bsp_pid BSP_FULL_NAMESPACE::ProcId
#define bsp_nprocs BSP_FULL_NAMESPACE::NProcs
#define bsp_abort BSP_FULL_NAMESPACE::Abort
#define bsp_vabort BSP_FULL_NAMESPACE::VAbort
#define bsp_sync BSP_FULL_NAMESPACE::Sync
#define bsp_time BSP_FULL_NAMESPACE::Time
#define bsp_push_reg BSP_FULL_NAMESPACE::Push
#define bsp_pop_reg BSP_FULL_NAMESPACE::Pop
#define bsp_put BSP_FULL_NAMESPACE::Put
#define bsp_get BSP_FULL_NAMESPACE::Get
#define bsp_set_tagsize BSP_FULL_NAMESPACE::SetTagSize
#define bsp_send BSP_FULL_NAMESPACE::Send
#define bsp_hpsend BSP_FULL_NAMESPACE::HPSend
#define bsp_qsize BSP_FULL_NAMESPACE::QSize
#define bsp_get_tag BSP_FULL_NAMESPACE::GetTag
#define bsp_move BSP_FULL_NAMESPACE::Move
#define bsp_hpmove BSP_FULL_NAMESPACE::HPMove
#define bsp_hpput BSP_FULL_NAMESPACE::HPPut
#define bsp_hpget BSP_FULL_NAMESPACE::HPGet

#endif

#endif