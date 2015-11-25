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

#include <type_traits>

#ifndef BSP_DISABLE_NAMESPACE
namespace BSPLib
{
#   define BSP_NAMESPACE BSPLib
#else
#   define BSP_NAMESPACE
#endif
    BSP_FORCEINLINE void Init( std::function< void() >spmd, int argc, char **argv )
    {
        BSP::GetInstance().Init( spmd, argc, argv );
    }

    BSP_FORCEINLINE void Begin( uint32_t P )
    {
        BSP::GetInstance().Begin( P );
    }

    BSP_FORCEINLINE void End()
    {
        BSP::GetInstance().End();
    }

    BSP_FORCEINLINE size_t ProcId()
    {
        return BSP::GetInstance().ProcId();
    }

    BSP_FORCEINLINE size_t NProcs()
    {
        return BSP::GetInstance().NProcs();
    }

    inline void Abort( const char *error_message, ... )
    {
        BSP::GetInstance().Abort( error_message );
    }

    BSP_FORCEINLINE void VAbort( const char *error_message, va_list args )
    {
        BSP::GetInstance().VAbort( error_message, args );
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

    BSP_FORCEINLINE void Send( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        BSP::GetInstance().Send( pid, tag, payload, size );
    }

    BSP_FORCEINLINE void HPSend( uint32_t pid, const void *tag, const void *payload, const size_t size )
    {
        BSP::GetInstance().Send( pid, tag, payload, size );
    }

    BSP_FORCEINLINE void QSize( size_t *packets, size_t *accumulated_size )
    {
        BSP::GetInstance().QSize( packets, accumulated_size );
    }

    BSP_FORCEINLINE void GetTag( size_t *status, void *tag )
    {
        BSP::GetInstance().GetTag( status, tag );
    }

    BSP_FORCEINLINE void Move( void *payload, const size_t max_copy_size )
    {
        BSP::GetInstance().Move( payload, max_copy_size );
    }

    BSP_FORCEINLINE size_t HPMove( void **tag_ptr, void **payload_ptr )
    {
        BSP &bsp = BSP::GetInstance();
        size_t status;
        bsp.GetTag( &status, *tag_ptr );
        bsp.Move( *payload_ptr, status );
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

    namespace Ext
    {
        template< typename tPrimitive >
        void PushReg( tPrimitive &ident )
        {
            ::PushReg( &ident, sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void PopReg( tPrimitive &ident )
        {
            ::PopReg( &ident );
        }

        template<>
        inline void PushReg( std::string &string )
        {
            BSP_NAMESPACE::PushReg( string.data(), string.size() );
        }

        template<>
        inline void PopReg( std::string &string )
        {
            BSP_NAMESPACE::PopReg( string.data() );
        }

        template< typename tPrimitive >
        void Put( uint32_t pid, tPrimitive &src, tPrimitive &dst )
        {
            ::Put( pid, &src, &dst, 0, sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void Get( uint32_t pid, tPrimitive &src, tPrimitive &dst )
        {
            ::Get( pid, &src, 0, &dst, sizeof( tPrimitive ) );
        }

        template< typename tPrimitive, typename tTag >
        void Send( uint32_t pid, const tTag &tag, const tPrimitive &payload )
        {
            Send( pid, &tag, &payload, sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void Move( tPrimitive &payload )
        {
            Move( &payload, sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void PushReg( tPrimitive *begin, tPrimitive *end )
        {
            PushReg( begin, ( end - begin ) * sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void PopReg( tPrimitive *begin )
        {
            PopReg( begin );
        }

        template< typename tPrimitive >
        void Put( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcEnd, tPrimitive *resultBegin, tPrimitive *resultDst )
        {
            Put( pid, srcBegin, resultBegin, ( resultDst - resultBegin ) * sizeof( tPrimitive ),
                 ( srcEnd - srcBegin ) * sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void Get( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcCursor, tPrimitive *resultBegin,
                  tPrimitive *resultEnd )
        {
            Get( pid, srcBegin, ( srcCursor - srcBegin ) * sizeof( tPrimitive ), resultBegin,
                 ( resultEnd - resultBegin ) * sizeof( tPrimitive ) );
        }

        template< typename tPrimitive, typename tTag >
        void Send( uint32_t pid, tTag *tag, tPrimitive *begin, tPrimitive *end )
        {
            Send( pid, tag, begin, ( end - begin )*sizeof( tPrimitive ) );
        }

        template< typename tPrimitive >
        void Move( tPrimitive *payload, uint32_t maxCopySize )
        {
            Move( payload, maxCopySize );
        }

        template< typename tIterator >
        void PushReg( tIterator begin, tIterator end )
        {
            assert( &*( end - 1 ) - &*begin == end - begin - 1 );
            PushReg( &*begin, ( end - begin ) * sizeof( tIterator::value_type ) );
        }

        template< typename tIterator>
        using IsIterator = std::integral_constant < bool,
              !std::is_same<typename std::iterator_traits<tIterator>::value_type, void>::value >;

        template < typename tIterator, typename = std::enable_if< IsIterator< tIterator >::value > >
        void PopReg( tIterator begin )
        {
            PopReg( &*begin );
        }

        template< typename tIterator, typename tOutputIterator >
        void Put( uint32_t pid, tIterator srcBegin, tIterator srcEnd, tOutputIterator resultBegin,
                  tOutputIterator resultDst )
        {
            assert( &*( srcEnd - 1 ) - &*srcBegin == srcEnd - srcBegin - 1 );
            assert( &*( resultDst - 1 ) - &*resultBegin == resultDst - resultBegin - 1 );
            Put( pid, &*srcBegin, &*srcEnd, &*resultBegin, &*resultDst );
        }

        template< typename tIterator, typename tOutputIterator >
        void Get( uint32_t pid, tIterator srcBegin, tIterator srcCursor, tOutputIterator resultBegin,
                  tOutputIterator resultEnd )
        {
            Get( pid, &*srcBegin, &*srcCursor, &*resultBegin, &*resultEnd );
        }

        template< typename tIterator, typename tTag >
        void Send( uint32_t pid, tTag *tag, tIterator begin, tIterator end )
        {
            Send( pid, tag, &*begin, &*end );
        }

        template< typename tIterator >
        void Move( tIterator begin, uint32_t maxCopySize )
        {
            Move( &begin, maxCopySize );
        }

        template< typename tContainer >
        using IsVector =
            std::is_same< tContainer, std::vector< typename tContainer::value_type, typename tContainer::allocator_type > >;

        template< typename tContainer, size_t N >
        using IsArray = std::is_same < tContainer, std::array< typename tContainer::value_type, N > >;

        template< typename tContainer, size_t N = 0 >
        using IsContainer = std::integral_constant < bool, IsVector< tContainer >::value || IsArray< tContainer, N >::value >;

        template< typename tContainer, typename = std::enable_if< IsContainer< tContainer >::value > >
        void PushReg( tContainer &container )
        {
            PushReg( pid, container.begin(), container.end() );
        }

        template< typename tContainer, typename = std::enable_if< IsContainer< tContainer >::value > >
        void PopReg( tContainer &container )
        {
            PopReg( container.begin() );
        }

        template < typename tContainerIn, typename tContainerOut,
                   typename = std::enable_if < IsContainer< tContainerIn >::value &&
                                               IsContainer< tContainerOut >::value > >
        void Put( uint32_t pid, tContainerIn &src, tContainerOut &dst )
        {
            Put( pid, src.begin(), src.end(), dst.begin(), dst.begin() );
        }

        template < typename tContainerIn, typename tContainerOut,
                   typename = std::enable_if < IsContainer< tContainerIn >::value &&
                                               IsContainer< tContainerOut >::value > >
        void Get( uint32_t pid, tContainerIn &src, tContainerOut &dst )
        {
            Get( pid, src.begin(), src.begin(), dst.begin(), dst.end() );
        }

        template< typename tTag, typename tContainer, typename = std::enable_if< IsContainer< tContainer >::value > >
        void Send( uint32_t pid, tTag *tag, tContainer &payload )
        {
            Send( pid, tag, payload.begin(), payload.end() );
        }

        template< typename tContainer, typename = std::enable_if< IsContainer< tContainer >::value > >
        void Move( tContainer &payload, uint32_t maxCopyIn )
        {
            Move( payload.begin(), payload.size() );
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
        void GetTag( size_t &status, tPrimitive *tag )
        {
            GetTag( &status, tag );
        }

        template< typename tIterator >
        void GetTag( size_t &status, tIterator tagBegin )
        {
            GetTag( status, &*tagBegin );
        }

        template< typename tContainer, typename = std::enable_if< IsContainer< tContainer > > >
        void GetTag( size_t &status, tContainer &tag )
        {
            GetTag( status, tag.begin() );
        }
    }
#ifndef BSP_DISABLE_NAMESPACE
}
#endif

#endif