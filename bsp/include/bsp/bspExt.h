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
#ifndef __BSPEXT_H__
#define __BSPEXT_H__

#include "bsp/bspClass.h"
#include "bsp/util.h"

#include <cstdint>

#ifndef BSP_DISABLE_NAMESPACE
namespace BSPLib
{
#   define BSP_NAMESPACE ::BSPLib
#else
#   define BSP_NAMESPACE
#endif

    namespace Classic
    {
        BSP_FORCEINLINE void Init( std::function< void() >spmd, int argc, char **argv )
        {
            BSP::GetInstance().Init( spmd, argc, argv );
        }

        BSP_FORCEINLINE void Begin( uint32_t p )
        {
            BSP::GetInstance().Begin( p );
        }

        BSP_FORCEINLINE void End()
        {
            BSP::GetInstance().End();
        }

        BSP_FORCEINLINE uint32_t ProcId()
        {
            return BSP::GetInstance().ProcId();
        }

        BSP_FORCEINLINE uint32_t NProcs()
        {
            return BSP::GetInstance().NProcs();
        }

        inline void Abort( const char *errorMessage, ... )
        {
            BSP::GetInstance().Abort( errorMessage );
        }

        BSP_FORCEINLINE void VAbort( const char *errorMessage, va_list args )
        {
            BSP::GetInstance().VAbort( errorMessage, args );
        }

        BSP_FORCEINLINE void Sync()
        {
            BSP::GetInstance().Sync();
        }

        BSP_FORCEINLINE double Time()
        {
            return BSP::GetInstance().Time();
        }

        BSP_FORCEINLINE void PushReg( const void *ident, size_t size )
        {
            BSP::GetInstance().PushReg( ident, size );
        }

        BSP_FORCEINLINE void PopReg( const void *ident )
        {
            BSP::GetInstance().PopReg( ident );
        }

        BSP_FORCEINLINE void Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
        {
            BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
        }

        BSP_FORCEINLINE void Get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
        {
            BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
        }

        BSP_FORCEINLINE void SetTagSize( size_t *size )
        {
            BSP::GetInstance().SetTagsize( size );
        }

        BSP_FORCEINLINE void Send( uint32_t pid, const void *tag, const void *payload, size_t size )
        {
            BSP::GetInstance().Send( pid, tag, payload, size );
        }

        BSP_FORCEINLINE void HPSend( uint32_t pid, const void *tag, const void *payload, size_t size )
        {
            BSP::GetInstance().Send( pid, tag, payload, size );
        }

        BSP_FORCEINLINE void QSize( size_t *packets, size_t *accumulatedSize )
        {
            BSP::GetInstance().QSize( packets, accumulatedSize );
        }

        BSP_FORCEINLINE void GetTag( size_t *status, void *tag )
        {
            BSP::GetInstance().GetTag( status, tag );
        }

        BSP_FORCEINLINE void Move( void *payload, size_t maxCopySize )
        {
            BSP::GetInstance().Move( payload, maxCopySize );
        }

        BSP_FORCEINLINE size_t HPMove( void **tagPtr, void **payloadPtr )
        {
            BSP &bsp = BSP::GetInstance();
            size_t status;
            bsp.GetTag( &status, *tagPtr );
            bsp.Move( *payloadPtr, status );
            return status;
        }

        BSP_FORCEINLINE void HPPut( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
        {
            BSP::GetInstance().Put( pid, src, dst, offset, nbytes );
        }

        BSP_FORCEINLINE void HPGet( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
        {
            BSP::GetInstance().Get( pid, src, offset, dst, nbytes );
        }
    }

    BSP_FORCEINLINE void Init( std::function< void() >spmd, int argc, char **argv )
    {
        Classic::Init( spmd, argc, argv );
    }

    BSP_FORCEINLINE void Begin( uint32_t p )
    {
        Classic::Begin( p );
    }

    BSP_FORCEINLINE void End()
    {
        Classic::End();
    }

    BSP_FORCEINLINE void Sync()
    {
        Classic::Sync();
    }

    BSP_FORCEINLINE uint32_t ProcId()
    {
        return Classic::ProcId();
    }

    BSP_FORCEINLINE uint32_t NProcs()
    {
        return Classic::NProcs();
    }

    BSP_FORCEINLINE double Time()
    {
        return Classic::Time();
    }

    template< typename tPrimitive >
    void PushReg( tPrimitive &ident )
    {
        Classic::PushReg( &ident, sizeof( tPrimitive ) );
    }

    inline void PushReg()
    {
        Classic::PushReg( nullptr, 0 );
    }

    template< typename tPrimitive >
    void PopReg( tPrimitive &ident )
    {
        Classic::PopReg( &ident );
    }

    inline void PopReg()
    {
        Classic::PopReg( nullptr );
    }

    template<>
    inline void PushReg( std::string &string )
    {
        Classic::PushReg( string.data(), string.size() );
    }

    template<>
    inline void PopReg( std::string &string )
    {
        Classic::PopReg( string.data() );
    }

    template< typename tPrimitive >
    void Put( uint32_t pid, tPrimitive &src, tPrimitive &dst )
    {
        Classic::Put( pid, &src, &dst, 0, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Put( uint32_t pid, tPrimitive &var )
    {
        Put( pid, var, var );
    }

    template< typename tPrimitive >
    void Get( uint32_t pid, tPrimitive &src, tPrimitive &dst )
    {
        Classic::Get( pid, &src, 0, &dst, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Get( uint32_t pid, tPrimitive &var )
    {
        Get( pid, var, var );
    }

    template< typename tPrimitive, typename tTag >
    void Send( uint32_t pid, const tTag &tag, const tPrimitive &payload )
    {
        Classic::Send( pid, &tag, &payload, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Move( tPrimitive &payload )
    {
        Classic::Move( &payload, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PushRegPtrs( tPrimitive *begin, tPrimitive *end )
    {
        Classic::PushReg( begin, ( end - begin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PopRegPtrs( tPrimitive *begin )
    {
        Classic::PopReg( begin );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcEnd, tPrimitive *resultBegin, tPrimitive *resultDst )
    {
        Classic::Put( pid, srcBegin, resultBegin, ( resultDst - resultBegin ) * sizeof( tPrimitive ),
                      ( srcEnd - srcBegin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *end )
    {
        PutPtrs( pid, begin, end, begin, begin );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcCursor, tPrimitive *resultBegin,
                  tPrimitive *resultEnd )
    {
        Classic::Get( pid, srcBegin, ( srcCursor - srcBegin ) * sizeof( tPrimitive ), resultBegin,
                      ( resultEnd - resultBegin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *cursor, tPrimitive *end )
    {
        GetPtrs( pid, begin, cursor, cursor, end );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *end )
    {
        GetPtrs( pid, begin, begin, end );
    }

    template< typename tPrimitive, typename tTag >
    void SendPtrs( uint32_t pid, tTag *tag, tPrimitive *begin, tPrimitive *end )
    {
        Classic::Send( pid, tag, begin, ( end - begin )*sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void MovePtrs( tPrimitive *payload, uint32_t maxCopySize )
    {
        Classic::Move( payload, maxCopySize );
    }

    template< typename tIterator >
    void PushRegIterator( tIterator begin, tIterator end )
    {
        assert( &*( end - 1 ) - &*begin == end - begin - 1 );
        PushRegPtrs( &*begin, &*end );
    }

    template < typename tIterator>
    void PopRegIterator( tIterator begin )
    {
        PopRegPtrs( &*begin );
    }

    template< typename tIterator, typename tOutputIterator >
    void PutIterator( uint32_t pid, tIterator srcBegin, tIterator srcEnd, tOutputIterator resultBegin,
                      tOutputIterator resultDst )
    {
        assert( &*( srcEnd - 1 ) - &*srcBegin == srcEnd - srcBegin - 1 );
        assert( &*( resultDst - 1 ) - &*resultBegin == resultDst - resultBegin - 1 );
        PutPtrs( pid, &*srcBegin, &*srcEnd, &*resultBegin, &*resultDst );
    }

    template< typename tIterator >
    void PutIterator( uint32_t pid, tIterator begin, tIterator cursor, tIterator end )
    {
        PutIterator( pid, begin, end, begin, cursor );
    }

    template< typename tIterator >
    void PutIterator( uint32_t pid, tIterator begin, tIterator end )
    {
        PutIterator( pid, begin, begin, end );
    }

    template< typename tIterator, typename tOutputIterator >
    void GetIterator( uint32_t pid, tIterator srcBegin, tIterator srcCursor, tOutputIterator resultBegin,
                      tOutputIterator resultEnd )
    {
        GetPtrs( pid, &*srcBegin, &*srcCursor, &*resultBegin, &*resultEnd );
    }

    template< typename tIterator >
    void GetIterator( uint32_t pid, tIterator begin, tIterator cursor, tIterator end )
    {
        GetIterator( pid, begin, cursor, begin, end );
    }

    template< typename tIterator >
    void GetIterator( uint32_t pid, tIterator begin, tIterator end )
    {
        GetIterator( pid, begin, begin, end );
    }

    template< typename tIterator, typename tTag >
    void SendIterator( uint32_t pid, tTag *tag, tIterator begin, tIterator end )
    {
        SendPtrs( pid, tag, &*begin, &*end );
    }

    template< typename tIterator >
    void MoveIterator( tIterator begin, uint32_t maxCopySize )
    {
        MovePtrs( &begin, maxCopySize );
    }

    template< typename tContainer >
    void PushRegContainer( tContainer &container )
    {
        PushRegIterator( container.begin(), container.end() );
    }

    template< typename tContainer >
    void PopRegContainer( tContainer &container )
    {
        PopRegIterator( container.begin() );
    }

    template < typename tContainerIn, typename tContainerOut >
    void PutContainer( uint32_t pid, tContainerIn &src, tContainerOut &dst )
    {
        PutIterator( pid, src.begin(), src.end(), dst.begin(), dst.begin() );
    }

    template< typename tContainer >
    void PutContainer( uint32_t pid, tContainer &container )
    {
        PutContainer( pid, container, container );
    }

    template < typename tContainerIn, typename tContainerOut >
    void GetContainer( uint32_t pid, tContainerIn &src, tContainerOut &dst )
    {
        GetIterator( pid, src.begin(), src.begin(), dst.begin(), dst.end() );
    }

    template< typename tContainer >
    void GetContainer( uint32_t pid, tContainer &container )
    {
        GetContainer( pid, container, container );
    }

    template< typename tTag, typename tContainer >
    void SendContainer( uint32_t pid, tTag *tag, tContainer &payload )
    {
        SendIterator( pid, tag, payload.begin(), payload.end() );
    }

    template< typename tContainer >
    void MoveContainer( tContainer &payload, uint32_t maxCopyIn )
    {
        MoveIterator( payload.begin(), payload.size() );
    }

    template< typename tPrimitive, size_t tSize >
    void PushRegContainer( tPrimitive container[tSize] )
    {
        PushRegPtrs( container, container + tSize );
    }

    template< typename tPrimitive, size_t tSize >
    void PopRegContainer( tPrimitive container[tSize] )
    {
        PopRegPtrs( container );
    }

    template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut  >
    void PutContainer( uint32_t pid, tPrimitive src[tSizeIn], tPrimitive dst[tSizeOut] )
    {
        PutPtrs( pid, src, src + tSizeIn, dst, dst );
    }

    template< typename tPrimitive, size_t tSize >
    void PutContainer( uint32_t pid, tPrimitive container[tSize] )
    {
        PutContainer( pid, container, container );
    }

    template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
    void GetContainer( uint32_t pid, tPrimitive src[tSizeIn], tPrimitive dst[tSizeOut] )
    {
        GetPtrs( pid, src, src, dst, dst + tSizeOut );
    }

    template< typename tPrimitive, size_t tSize >
    void GetContainer( uint32_t pid, tPrimitive container[tSize] )
    {
        GetContainer( pid, container, container );
    }

    template< typename tPrimitive, typename tTag, size_t tSize >
    void SendContainer( uint32_t pid, tTag *tag, tPrimitive payload[tSize] )
    {
        SendPtrs( pid, tag, payload, payload + tSize )
    }

    template< typename tPrimitive, size_t tSize >
    void MoveContainer( tPrimitive payload[tSize], uint32_t maxCopyIn )
    {
        MovePtrs( payload, maxCopyIn );
    }

    template< typename tPrimitive >
    void SetTagsize()
    {
        SetTagsize( sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void SetTagsize( uint32_t count )
    {
        SetTagsize( count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void GetTag( size_t &status, tPrimitive &tag )
    {
        GetTag( &status, &tag );
    }

    template< typename tPrimitive >
    void GetTagPtr( size_t &status, tPrimitive *tag )
    {
        GetTag( &status, tag );
    }

    template< typename tIterator >
    void GetTagIterator( size_t &status, tIterator tagBegin )
    {
        GetTag( status, &*tagBegin );
    }

    template< typename tContainer >
    void GetTagContainer( size_t &status, tContainer &tag )
    {
        GetTag( status, tag.begin() );
    }
#ifndef BSP_DISABLE_NAMESPACE
}
#endif

#endif