#Interfaces

```cpp
template< typename tIterator, typename tOutputIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator srcBegin, tIterator srcCursor, tOutputIterator resultBegin,
                    tOutputIterator resultEnd )

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin, tIterator cursor, tIterator end )

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin, tIterator end )

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin, size_t offset, size_t count )

template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
void BSPLib::GetCArray( uint32_t pid, tPrimitive( &src )[tSizeIn], tPrimitive( &dst )[tSizeOut] )

template < typename tContainerIn, typename tContainerOut >
void BSPLib::GetContainer( uint32_t pid, tContainerIn &src, tContainerOut &dst )    

template< typename tPrimitive, size_t tSize >
void BSPLib::GetCArray( uint32_t pid, tPrimitive( &container )[tSize] )
```

Pushes a register, with the given size. 

1. Modern interface of the classic BSP function.

!!! danger "Warnings"
     * In case of (4), the std::string must not be resized.


#Parameters

* `ident` The address to register.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* Push request has been queued.
* In the next superstep, this register will be available for [`BSPLib::Put()`](../com/put.md)/[`BSPLib::Get()`](../com/get.md).
     
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.