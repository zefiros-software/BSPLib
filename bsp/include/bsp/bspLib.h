#pragma once
#ifndef __BSP_H__
#define __BSP_H__

#ifndef DEBUG
#define BSP_SKIP_CHECKS
#endif

#include "bsp/bspExt.h"

#ifndef BSP_DISABLE_LEGACY

#define bsp_init ::BSP_NAMESPACE::Init
#define bsp_begin ::BSP_NAMESPACE::Begin
#define bsp_end ::BSP_NAMESPACE::End
#define bsp_pid ::BSP_NAMESPACE::ProcId
#define bsp_nprocs ::BSP_NAMESPACE::NProcs
#define bsp_abort ::BSP_NAMESPACE::Abort
#define bsp_vabort ::BSP_NAMESPACE::VAbort
#define bsp_sync ::BSP_NAMESPACE::Sync
#define bsp_time ::BSP_NAMESPACE::Time
#define bsp_push_reg ::BSP_NAMESPACE::PushReg
#define bsp_pop_reg ::BSP_NAMESPACE::PopReg
#define bsp_put ::BSP_NAMESPACE::Put
#define bsp_get ::BSP_NAMESPACE::Get
#define bsp_set_tagsize ::BSP_NAMESPACE::SetTagSize
#define bsp_send ::BSP_NAMESPACE::Send
#define bsp_hpsend ::BSP_NAMESPACE::HPSend
#define bsp_qsize ::BSP_NAMESPACE::QSize
#define bsp_get_tag ::BSP_NAMESPACE::GetTag
#define bsp_move ::BSP_NAMESPACE::Move
#define bsp_hpmove ::BSP_NAMESPACE::HPMove
#define bsp_hpput ::BSP_NAMESPACE::HPPut
#define bsp_hpget ::BSP_NAMESPACE::HPGet

#endif

#endif