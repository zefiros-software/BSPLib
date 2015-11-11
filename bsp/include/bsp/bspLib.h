#pragma once
#ifndef __BSP_H__
#define __BSP_H__

#ifndef DEBUG
#define SKIP_CHECKS
#endif

#include "bsp/bspClass.h"

BSP_FORCEINLINE void bsp_init( void( *spmd )( void ), int argc, char **argv )
{
    BSP::GetInstance().Init( spmd, argc, argv );
}

BSP_FORCEINLINE void bsp_begin( size_t P )
{
    BSP::GetInstance().Begin( P );
}

BSP_FORCEINLINE void bsp_end()
{
    BSP::GetInstance().End();
}

BSP_FORCEINLINE size_t bsp_pid()
{
    return BSP::GetInstance().PID();
}

BSP_FORCEINLINE size_t bsp_nprocs()
{
    return BSP::GetInstance().NProcs();
}

BSP_FORCEINLINE void bsp_abort( const char *error_message, ... )
{
    BSP::GetInstance().Abort( error_message );
}

BSP_FORCEINLINE void bsp_vabort( const char *error_message, va_list args )
{
    BSP::GetInstance().VAbort( error_message, args );
}

BSP_FORCEINLINE void bsp_sync()
{
    BSP::GetInstance().Sync();
}

BSP_FORCEINLINE double bsp_time()
{
    return BSP::GetInstance().Time();
}

BSP_FORCEINLINE void bsp_push_reg( const void *ident, size_t size )
{
    BSP::GetInstance().PushReg( ident, size );
}

BSP_FORCEINLINE void bsp_pop_reg( const void *ident )
{
    BSP::GetInstance().PopReg( ident );
}

BSP_FORCEINLINE void bsp_put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{
    BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
}

BSP_FORCEINLINE void bsp_get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{
    BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
}

BSP_FORCEINLINE void bsp_set_tagsize( size_t *size )
{
    BSP::GetInstance().SetTagsize( size );
}

BSP_FORCEINLINE void bsp_send( uint32_t pid, const void *tag, const void *payload, const size_t size )
{
    BSP::GetInstance().Send( pid, tag, payload, size );
}

BSP_FORCEINLINE void bsp_hpsend( uint32_t pid, const void *tag, const void *payload, const size_t size )
{
    BSP::GetInstance().Send( pid, tag, payload, size );
}

BSP_FORCEINLINE void bsp_qsize( size_t *packets, size_t *accumulated_size )
{
    BSP::GetInstance().QSize( packets, accumulated_size );
}

BSP_FORCEINLINE void bsp_get_tag( size_t *status, void *tag )
{
    BSP::GetInstance().GetTag( status, tag );
}

BSP_FORCEINLINE void bsp_move( void *payload, const size_t max_copy_size )
{
    BSP::GetInstance().Move( payload, max_copy_size );
}

BSP_FORCEINLINE size_t bsp_hpmove( void **tag_ptr, void **payload_ptr )
{
    BSP &bsp = BSP::GetInstance();
    size_t status;
    bsp.GetTag( &status, *tag_ptr );
    bsp.Move( *payload_ptr, status );
    return status;
}

BSP_FORCEINLINE void bsp_hpput( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
{
    BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
}

BSP_FORCEINLINE void bsp_hpget( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
{
    BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
}

#endif