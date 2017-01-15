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
#ifndef __BSPLIB_BSPEXT_H__
#define __BSPLIB_BSPEXT_H__

#include "bsp/bspClass.h"
#include "bsp/bspProf.h"
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
        BSP_FORCEINLINE void Init( std::function< void() >spmd, int32_t argc, char **argv )
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

        BSP_FORCEINLINE void Push( const void *ident, size_t size )
        {
            BSP::GetInstance().PushReg( ident, size );
        }

        BSP_FORCEINLINE void Pop( const void *ident )
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

    BSP_FORCEINLINE void Sync()
    {
        Classic::Sync();
    }

    BSP_FORCEINLINE void SyncPutRequests()
    {
        BSP::GetInstance().SyncPutRequests();
    }

    BSP_FORCEINLINE void SyncGetRequests()
    {
        BSP::GetInstance().SyncGetRequests();
    }

    BSP_FORCEINLINE void SyncSendRequests()
    {
        BSP::GetInstance().SyncSendRequests();
    }

    BSP_FORCEINLINE void SyncPoint()
    {
        BSP::GetInstance().SyncPoint();
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

    BSP_FORCEINLINE BSPUtil::TicTimer Tic()
    {
        return BSP::GetInstance().Tic();
    }

    BSP_FORCEINLINE double Toc()
    {
        return BSP::GetInstance().Toc();
    }

    template< typename tPrimitive >
    void Push( tPrimitive &ident )
    {
        Classic::Push( &ident, sizeof( tPrimitive ) );
    }

    inline void Push( const void *ident, size_t byteSize )
    {
        Classic::Push( ident, byteSize );
    }

    inline void Push()
    {
        Classic::Push( nullptr, 0 );
    }

    template< typename tPrimitive >
    void Pop( tPrimitive &ident )
    {
        Classic::Pop( &ident );
    }

    inline void Pop( const void *ident )
    {
        Classic::Pop( ident );
    }

    inline void Pop()
    {
        Classic::Pop( nullptr );
    }

    template<>
    inline void Push( std::string &string )
    {
        Classic::Push( string.data(), string.size() );
    }

    template<>
    inline void Pop( std::string &string )
    {
        Classic::Pop( string.data() );
    }

    template< typename tPrimitive >
    void Put( uint32_t pid, tPrimitive &src, tPrimitive &dst )
    {
        Classic::Put( pid, &src, &dst, 0, sizeof( tPrimitive ) );
    }

    template <>
    inline void Put( uint32_t pid, std::string &src, std::string &dst )
    {
        Classic::Put( pid, src.data(), &*dst.begin(), 0, src.size() );
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

    template<>
    inline void Get( uint32_t pid, std::string &src, std::string &dst )
    {
        Classic::Get( pid, &*src.begin(), 0, &*dst.begin(), dst.size() );
    }

    template< typename tPrimitive >
    void Get( uint32_t pid, tPrimitive &var )
    {
        Get( pid, var, var );
    }

    template< typename tPrimitive >
    void Move( tPrimitive &payload )
    {
        Classic::Move( &payload, sizeof( tPrimitive ) );
    }

    template<>
    inline void Move( std::string &payload )
    {
        Classic::Move( &*payload.begin(), payload.size() );
    }

    template< typename tPrimitive >
    void PushPtrs( tPrimitive *begin, size_t count )
    {
        Classic::Push( begin, count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PushPtrs( tPrimitive *begin, tPrimitive *end )
    {
        PushPtrs( begin, end - begin );
    }

    template< typename tPrimitive >
    void PopPtrs( tPrimitive *begin )
    {
        Classic::Pop( begin );
    }

    template< typename tPrimitive >
    BSP_FORCEINLINE void PutPtrs( uint32_t pid, tPrimitive *srcBegin, size_t count, tPrimitive *resultBegin, size_t offset )
    {
        Classic::Put( pid, srcBegin, resultBegin, offset * sizeof( tPrimitive ), count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcEnd, tPrimitive *resultBegin, tPrimitive *resultDst )
    {
        PutPtrs( pid, srcBegin, srcEnd - srcBegin, resultBegin, resultDst - resultBegin );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *end )
    {
        PutPtrs( pid, begin, end, begin, begin );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *cursor, tPrimitive *end )
    {
        PutPtrs( pid, cursor, end, begin, cursor );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *begin, size_t count )
    {
        PutPtrs( pid, begin, begin, begin + count );
    }

    template< typename tPrimitive >
    void PutPtrs( uint32_t pid, tPrimitive *begin, size_t offset, size_t count )
    {
        PutPtrs( pid, begin + offset, count, begin, offset );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcCursor, tPrimitive *resultBegin,
                  tPrimitive *resultEnd )
    {
        Classic::Get( pid, srcBegin, ( srcCursor - srcBegin ) * sizeof( tPrimitive ), resultBegin,
                      ( resultEnd - resultBegin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *srcBegin, size_t offset, tPrimitive *resultBegin, size_t count )
    {
        GetPtrs( pid, srcBegin, srcBegin + offset, resultBegin, resultBegin + count );
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

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *begin, size_t offset, size_t count )
    {
        GetPtrs( pid, begin, offset, begin + offset, count );
    }

    template< typename tPrimitive >
    void GetPtrs( uint32_t pid, tPrimitive *begin, size_t count )
    {
        GetPtrs( pid, begin, begin, begin + count );
    }

    template< typename tPrimitive, typename tTagPrimitive >
    void SendPtrs( uint32_t pid, tTagPrimitive *tag, tPrimitive *begin, size_t count )
    {
        Classic::Send( pid, tag, begin, count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive, typename tTagPrimitive >
    void SendPtrs( uint32_t pid, tTagPrimitive *tag, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, tag, begin, end - begin );
    }

    template< typename tPrimitive, typename tTagPrimitive >
    void SendPtrs( uint32_t pid, std::vector< tTagPrimitive > &tagContainer, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, tagContainer.data(), begin, end );
    }

    template< typename tPrimitive, typename tTagPrimitive >
    void SendPtrs( uint32_t pid, std::vector< tTagPrimitive > &tagContainer, tPrimitive *begin, size_t count )
    {
        SendPtrs( pid, tagContainer.data(), begin, count );
    }

    template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
    void SendPtrs( uint32_t pid, std::array< tTagPrimitive, tTagSize > &tagContainer, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, tagContainer.data(), begin, end );
    }

    template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
    void SendPtrs( uint32_t pid, std::array< tTagPrimitive, tTagSize > &tagContainer, tPrimitive *begin, size_t count )
    {
        SendPtrs( pid, tagContainer.data(), begin, count );
    }

    template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
    void SendPtrsWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, tagContainer, begin, end - begin );
    }

    template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
    void SendPtrsWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], tPrimitive *begin, size_t count )
    {
        SendPtrs( pid, tagContainer, begin, count );
    }

    template< typename tPrimitive, typename tTag >
    void SendPtrs( uint32_t pid, const tTag &tag, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, &tag, begin, end );
    }

    template< typename tPrimitive, typename tTag >
    void SendPtrs( uint32_t pid, const tTag &tag, tPrimitive *begin, size_t count )
    {
        SendPtrs( pid, &tag, begin, count );
    }

    template< typename tPrimitive >
    void SendPtrs( uint32_t pid, const std::string &tag, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, tag.data(), begin, end );
    }

    template< typename tPrimitive >
    void SendPtrs( uint32_t pid, const std::string &tag, tPrimitive *begin, size_t count )
    {
        SendPtrs( pid, tag.data(), begin, count );
    }




    template< typename tPrimitive >
    void SendPtrs( uint32_t pid, tPrimitive *begin, size_t count )
    {
        Classic::Send( pid, nullptr, begin, count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void SendPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *end )
    {
        SendPtrs( pid, nullptr, begin, end - begin );
    }



    template< typename tPrimitive >
    void MovePtrs( tPrimitive *payload, uint32_t maxCopySize )
    {
        Classic::Move( payload, maxCopySize * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void MovePtrs( tPrimitive *begin, tPrimitive *end )
    {
        MovePtrs( begin, ( uint32_t )( end - begin ) );
    }

    template< typename tIterator >
    void PushIterator( tIterator begin, size_t count )
    {
        PushPtrs( &*begin, count );
    }

    template< typename tIterator >
    void PushIterator( tIterator begin, tIterator end )
    {
        assert( end == begin || &*( end - 1 ) - &*begin == end - begin - 1 );
        PushIterator( begin, end - begin );
    }

    template < typename tIterator>
    void PopIterator( tIterator begin )
    {
        PopPtrs( &*begin );
    }

    template< typename tIterator, typename tOutputIterator >
    BSP_FORCEINLINE void PutIterator( uint32_t pid, tIterator srcBegin, size_t count, tOutputIterator resultBegin,
                                      size_t offset )
    {
        PutPtrs( pid, &*srcBegin, count, &*resultBegin, offset );
    }

    template< typename tIterator, typename tOutputIterator >
    void PutIterator( uint32_t pid, tIterator srcBegin, tIterator srcEnd, tOutputIterator resultBegin,
                      tOutputIterator resultDst )
    {
        assert( srcEnd == srcBegin || &*( srcEnd - 1 ) - &*srcBegin == srcEnd - srcBegin - 1 );
        assert( resultDst == resultBegin || &*( resultDst - 1 ) - &*resultBegin == resultDst - resultBegin - 1 );
        PutIterator( pid, srcBegin, srcEnd - srcBegin, resultBegin, resultDst - resultBegin );
    }

    template< typename tIterator >
    void PutIterator( uint32_t pid, tIterator begin, tIterator cursor, tIterator end )
    {
        PutIterator( pid, cursor, end, begin, cursor );
    }

    template< typename tIterator >
    void PutIterator( uint32_t pid, tIterator begin, tIterator end )
    {
        PutIterator( pid, begin, begin, end );
    }

    template< typename tIterator >
    void PutIterator( uint32_t pid, tIterator begin, size_t offset, size_t count )
    {
        PutIterator( pid, begin + offset, begin + offset + count, begin, begin + offset );
    }

    template< typename tIterator, typename tOutputIterator >
    void GetIterator( uint32_t pid, tIterator srcBegin, size_t offset, tOutputIterator resultBegin, size_t count )
    {
        GetPtrs( pid, &*srcBegin, offset, &*resultBegin, count );
    }

    template< typename tIterator, typename tOutputIterator >
    void GetIterator( uint32_t pid, tIterator srcBegin, tIterator srcCursor, tOutputIterator resultBegin,
                      tOutputIterator resultEnd )
    {
        GetIterator( pid, srcBegin, srcCursor - srcBegin, resultBegin, resultEnd - resultBegin );
    }

    template< typename tIterator >
    void GetIterator( uint32_t pid, tIterator begin, tIterator cursor, tIterator end )
    {
        GetIterator( pid, begin, cursor, cursor, end );
    }

    template< typename tIterator >
    void GetIterator( uint32_t pid, tIterator begin, tIterator end )
    {
        GetIterator( pid, begin, begin, end );
    }

    template< typename tIterator >
    void GetIterator( uint32_t pid, tIterator begin, size_t offset, size_t count )
    {
        GetIterator( pid, begin, begin + offset, begin + offset, begin + offset + count );
    }

    template< typename tIterator, typename tTag >
    void SendIterator( uint32_t pid, tTag &tag, tIterator begin, size_t count )
    {
        SendPtrs( pid, tag, &*begin, count );
    }

    template< typename tIterator, typename tTag >
    void SendIterator( uint32_t pid, tTag *tag, tIterator begin, size_t count )
    {
        SendPtrs( pid, tag, &*begin, count );
    }

    template< typename tIterator, typename tTag >
    void SendIterator( uint32_t pid, tTag &tag, tIterator begin, tIterator end )
    {
        SendIterator( pid, tag, begin, end - begin );
    }

    template< typename tIterator, typename tTag >
    void SendIterator( uint32_t pid, tTag *tag, tIterator begin, tIterator end )
    {
        SendIterator( pid, tag, begin, end - begin );
    }

    template< typename tIterator, typename tTagPrimitive, uint32_t tTagSize >
    void SendIteratorWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], tIterator begin, size_t count )
    {
        SendPtrsWithCArray( pid, tagContainer, &*begin, count );
    }

    template< typename tIterator, typename tTagPrimitive, uint32_t tTagSize >
    void SendIteratorWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], tIterator begin, tIterator end )
    {
        SendIteratorWithCArray( pid, tagContainer, begin, end - begin );
    }

    template< typename tIterator >
    void SendIterator( uint32_t pid, tIterator begin, size_t count )
    {
        SendPtrs( pid, &*begin, count );
    }

    template< typename tIterator >
    void SendIterator( uint32_t pid, tIterator begin, tIterator end )
    {
        SendIterator( pid, begin, ( uint32_t )( end - begin ) );
    }

    template< typename tIterator >
    void MoveIterator( tIterator begin, uint32_t maxCopySize )
    {
        MovePtrs( &*begin, maxCopySize );
    }

    template< typename tIterator >
    void MoveIterator( tIterator begin, tIterator end )
    {
        MoveIterator( begin, ( uint32_t )( end - begin ) );
    }

    template< typename tPrimitive, size_t tSize >
    void PushCArray( tPrimitive( &container )[tSize] )
    {
        PushPtrs( container, container + tSize );
    }

    template< typename tPrimitive, size_t tSize >
    void PopCArray( tPrimitive( &container )[tSize] )
    {
        PopPtrs( container );
    }

    template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut  >
    void PutCArray( uint32_t pid, tPrimitive( &src )[tSizeIn], tPrimitive( &dst )[tSizeOut] )
    {
        PutPtrs( pid, src, src + tSizeIn, dst, dst );
    }

    template< typename tPrimitive, size_t tSize >
    void PutCArray( uint32_t pid, tPrimitive( &container )[tSize] )
    {
        PutCArray( pid, container, container );
    }

    template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
    void GetCArray( uint32_t pid, tPrimitive( &src )[tSizeIn], tPrimitive( &dst )[tSizeOut] )
    {
        GetPtrs( pid, src, src, dst, dst + tSizeOut );
    }

    template< typename tPrimitive, size_t tSize >
    void GetCArray( uint32_t pid, tPrimitive( &container )[tSize] )
    {
        GetCArray( pid, container, container );
    }

    template< typename tPrimitive, typename tTag, size_t tSize >
    void SendCArray( uint32_t pid, tTag *tag, tPrimitive( &payload )[tSize] )
    {
        SendPtrs( pid, tag, payload, tSize );
    }

    template< typename tPrimitive, typename tTag, size_t tSize >
    void SendCArray( uint32_t pid, tTag &tag, tPrimitive( &payload )[tSize] )
    {
        SendPtrs( pid, tag, payload, tSize );
    }

    template< typename tPrimitive, typename tTagPrimitive, size_t tSize, size_t tTagSize >
    void SendCArrayWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], tPrimitive( &payload )[tSize] )
    {
        SendPtrsWithCArray( pid, tagContainer, payload, tSize );
    }

    template< typename tPrimitive, size_t tSize >
    void SendCArray( uint32_t pid, tPrimitive( &payload )[tSize] )
    {
        SendPtrs( pid, payload, tSize );
    }

    template< typename tPrimitive, size_t tSize >
    void MoveCArray( tPrimitive( &payload )[tSize] )
    {
        MovePtrs( payload, tSize );
    }

    template< typename tContainer >
    void PushContainer( tContainer &container )
    {
        PushIterator( container.begin(), container.end() );
    }

    template< typename tContainer >
    void PopContainer( tContainer &container )
    {
        PopIterator( container.begin() );
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

    template< typename tTag, typename tContainer >
    void SendContainer( uint32_t pid, tTag &tag, tContainer &payload )
    {
        SendIterator( pid, tag, payload.begin(), payload.end() );
    }

    template< typename tTagPrimitive, typename tContainer, size_t tTagSize >
    void SendContainerWithCArray( uint32_t pid, tTagPrimitive( &tag )[tTagSize], tContainer &payload )
    {
        SendIteratorWithCArray( pid, tag, payload.begin(), payload.end() );
    }

    template< typename tContainer >
    void SendContainer( uint32_t pid, tContainer &payload )
    {
        SendIterator( pid, payload.begin(), payload.end() );
    }

    template< typename tContainer >
    void MoveContainer( tContainer &payload )
    {
        MoveIterator( payload.begin(), ( uint32_t )payload.size() );
    }

    template< typename tPrimitive >
    void SetTagsize()
    {
        size_t size = sizeof( tPrimitive );
        Classic::SetTagSize( &size );
    }

    template< typename tPrimitive >
    void SetTagsize( uint32_t count )
    {
        size_t size = count * sizeof( tPrimitive );
        Classic::SetTagSize( &size );
    }

    inline void QSize( size_t &packets )
    {
        Classic::QSize( &packets, nullptr );
    }

    inline void QSize( size_t &packets, size_t &accumulatedSize )
    {
        Classic::QSize( &packets, &accumulatedSize );
    }

    template< typename tPrimitive >
    void GetTag( size_t &status, tPrimitive &tag )
    {
        Classic::GetTag( &status, &tag );
    }

    template<>
    inline void GetTag( size_t &status, std::string &tag )
    {
        Classic::GetTag( &status, &*tag.begin() );
    }

    template< typename tPrimitive >
    void GetTagPtr( size_t &status, tPrimitive *tag )
    {
        GetTag( status, *tag );
    }

    template< typename tIterator >
    void GetTagIterator( size_t &status, tIterator tagBegin )
    {
        GetTagPtr( status, &*tagBegin );
    }

    template< typename tContainer >
    void GetTagContainer( size_t &status, tContainer &tag )
    {
        GetTagIterator( status, tag.begin() );
    }

    template< typename tPrimitive, size_t tSize >
    void GetTagCArray( size_t &status, tPrimitive( &tag )[tSize] )
    {
        GetTagPtr( status, tag );
    }

    /**
     * Executes the by func given BSP program.
     *
     * @param   func  The function to execute BSP style.
     * @param   nProc The number of processors to use
     * @param   argc  The argc argument from the main loop.
     * @param   argv  The argv argument from the main loop.
     *
     * @return true if it succeeds, false if it fails.
     */

    inline bool Execute( std::function< void() > func, uint32_t nProc, int32_t argc, char **argv )
    {
        std::function< void() > spmd = [func, nProc]
        {
            BSPLib::Classic::Begin( nProc );

            func();

            BSPLib::Classic::End();
        };

        BSPLib::Classic::Init( spmd, argc, argv );

        try
        {
            spmd();
        }
        catch ( BSPInternal::BspAbort & )
        {
            return false;
        }

        return true;
    }

    /**
     * Executes the by func given BSP program.
     *
     * @param   func  The function to execute BSP style.
     * @param   nProc The number of processors to use.
     *
     * @return true if it succeeds, false if it fails.
     */

    inline bool Execute( std::function< void() > func, uint32_t nProc )
    {
        return Execute( func, nProc, 0, nullptr );
    }




    template< typename tPrimitive, typename tTag >
    void Send( uint32_t pid, tTag &tag, const tPrimitive &payload )
    {
        SendPtrs( pid, tag, &payload, 1 );
    }

    template< typename tTag >
    void Send( uint32_t pid, tTag &tag, const std::string &payload )
    {
        SendPtrs( pid, tag, payload.data(), payload.size() );
    }

    template< typename tPrimitive, typename tTag >
    void Send( uint32_t pid, tTag *tag, const tPrimitive &payload )
    {
        SendPtrs( pid, tag, &payload, 1 );
    }

    template< typename tTag >
    void Send( uint32_t pid, tTag *tag, const std::string &payload )
    {
        SendPtrs( pid, tag, payload.data(), payload.size() );
    }

    template< typename tPrimitive >
    void Send( uint32_t pid, const tPrimitive &payload )
    {
        SendPtrs( pid, &payload, 1 );
    }

    template<>
    inline void Send( uint32_t pid, const std::string &payload )
    {
        SendPtrs( pid, payload.data(), payload.size() );
    }



    template< typename tTagPrimitive, size_t tTagSize >
    void SendWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], const std::string &payload )
    {
        SendPtrs( pid, tagContainer, payload.data(), payload.size() );
    }

    template< typename tPrimitive, typename tTagPrimitive, size_t tTagSize >
    void SendWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], const tPrimitive &payload )
    {
        SendPtrs( pid, tagContainer, &payload, 1 );
    }

#ifndef BSP_DISABLE_NAMESPACE
}
#endif

#endif