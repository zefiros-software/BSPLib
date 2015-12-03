#Interfaces

```cpp
void BSPLib::Pop( const void *ident )			            // (1) Modern
void BSPLib::Pop()			                                // (1) Modern

template< typename tPrimitive >
void BSPLib::Pop( tPrimitive &ident )                       //

template<>
void BSPLib::Pop( std::string &string )                     //

template< typename tPrimitive >
void BSPLib::PopPtrs( tPrimitive *begin )                   //

template< typename tPrimitive, size_t tSize >
void BSPLib::PopCArray( tPrimitive( &container )[tSize] )   //

template< typename tContainer >
void BSPLib::PopContainer( tContainer &container )          //

template < typename tIterator>
void BSPLib::PopIterator( tIterator begin )                 //

void BSPLib::Classic::Pop( const void *ident )              // (2) Classic
void bsp_popreg( const void *ident )			            // (3) BSP
```

Pops the register described by `ident`.

#Parameters

* `ident` The buffer pointer to register.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* Pop request has been queued.
* In the next superstep, this register will be unavailable for [`BSPLib::Put()`](../com/put.md)/[`BSPLib::Get()`](../com/get.md).
	 
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.