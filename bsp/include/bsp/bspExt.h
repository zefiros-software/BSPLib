#pragma once
#ifndef __BSPEXT_H__
#define __BSPEXT_H__

#include "bsp/bspLib.h"

#include <type_traits>

namespace BSPLib
{
    template< typename tPrimitive >
    void PushReg( tPrimitive &ident )
    {
        ::bsp_push_reg( &ident, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PopReg( tPrimitive &ident )
    {
        ::bsp_pop_reg( &ident );
    }

    template<>
    void PushReg( std::string &string )
    {
        ::bsp_push_reg( string.data(), string.size() );
    }

    template<>
    void PopReg( std::string &string )
    {
        ::bsp_pop_reg( string.data() );
    }

    template< typename tPrimitive >
    void Put( uint32_t pid, tPrimitive &src, tPrimitive &dst )
    {
        ::bsp_put( pid, &src, &dst, 0, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Get( uint32_t pid, tPrimitive &src, tPrimitive &dst )
    {
        ::bsp_get( pid, &src, 0, &dst, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive, typename tTag >
    void Send( uint32_t pid, const tTag &tag, const tPrimitive &payload )
    {
        ::bsp_send( pid, &tag, &payload, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Move( tPrimitive &payload )
    {
        ::bsp_move( &payload, sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PushReg( tPrimitive *begin, tPrimitive *end )
    {
        ::bsp_push_reg( begin, ( end - begin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void PopReg( tPrimitive *begin )
    {
        ::bsp_pop_reg( begin );
    }

    template< typename tPrimitive >
    void Put( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcEnd, tPrimitive *resultBegin, tPrimitive *resultDst )
    {
        ::bsp_put( pid, srcBegin, resultBegin, ( resultDst - resultBegin ) * sizeof( tPrimitive ),
                   ( srcEnd - srcBegin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Get( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcCursor, tPrimitive *resultBegin,
              tPrimitive *resultEnd )
    {
        ::bsp_get( pid, srcBegin, ( srcCursor - srcBegin ) * sizeof( tPrimitive ), resultBegin,
                   ( resultEnd - resultBegin ) * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive, typename tTag >
    void Send( uint32_t pid, tTag *tag, tPrimitive *begin, tPrimitive *end )
    {
        ::bsp_send( pid, tag, begin, ( end - begin )*sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void Move( tPrimitive *payload, uint32_t maxCopySize )
    {
        ::bsp_move( payload, maxCopySize );
    }

    template< typename tIterator >
    void PushReg( tIterator begin, tIterator end )
    {
        assert( &*( end - 1 ) - &*begin == end - begin - 1 );
        ::bsp_push_reg( &*begin, ( end - begin ) * sizeof( tIterator::value_type ) );
    }

    template< typename tIterator>
    using IsIterator = std::integral_constant < bool,
          !std::is_same<typename std::iterator_traits<tIterator>::value_type, void>::value >;

    template < typename tIterator, typename = std::enable_if< IsIterator< tIterator >::value > >
    void PopReg( tIterator begin )
    {
        ::bsp_pop_reg( &*begin );
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
        ::bsp_set_tagsize( sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void SetTagsize( uint32_t count )
    {
        ::bsp_set_tagsize( count * sizeof( tPrimitive ) );
    }

    template< typename tPrimitive >
    void GetTag( size_t &status, tPrimitive &tag )
    {
        ::bsp_get_tag( &status, &tag );
    }

    template< typename tPrimitive >
    void GetTag( size_t &status, tPrimitive *tag )
    {
        ::bsp_get_tag( &status, tag );
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

#endif