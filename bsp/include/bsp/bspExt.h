#pragma once
#ifndef __BSPEXT_H__
#define __BSPEXT_H__

#include "bsp/bspLib.h"

template< typename tPrimitive >
void bsp_push_reg( tPrimitive &ident )
{
    bsp_push_reg( &ident, sizeof( tPrimitive ) );
}

template<>
void bsp_push_reg( std::string &string )
{
    bsp_push_reg( string.data(), string.size() );
}

template< typename tPrimitive >
void bsp_pop_reg( tPrimitive &ident )
{
    bsp_pop_reg( &ident );
}

template< typename tPrimitive >
void bsp_put( uint32_t pid, tPrimitive &src, tPrimitive &dst )
{
    bsp_put( pid, &src, &dst, 0, sizeof( tPrimitive ) );
}

template< typename tPrimitive, typename tTag >
void bsp_send( uint32_t pid, const tTag &tag, const tPrimitive &payload )
{
    bsp_send( pid, &tag, &payload, sizeof( tPrimitive ) );
}

template< typename tPrimitive >
void bsp_move( tPrimitive &payload )
{
    bsp_move( &payload, sizeof( tPrimitive ) );
}

#endif