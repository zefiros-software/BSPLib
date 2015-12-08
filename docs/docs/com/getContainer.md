#Interfaces

```cpp
template< typename tIterator, typename tOutputIterator >
    void GetIterator( uint32_t pid, tIterator srcBegin, size_t offset, 
                      tOutputIterator resultBegin, 
                      size_t count )                            // (1) Begin-Offset-Count

template< typename tIterator, typename tOutputIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator srcBegin, tIterator srcCursor, 
                          tOutputIterator resultBegin,
                          tOutputIterator resultEnd )           // (2) Begin-Cursor-End

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin, 
                          tIterator cursor, tIterator end )     // (3) Same Begin-Cursor-End

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin, 
                          tIterator end )                       // (4) Same Begin-End

template< typename tIterator >
void BSPLib::GetIterator( uint32_t pid, tIterator begin,
                          size_t offset, size_t count )         // (5) Same Begin-Offset-Count

template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
void BSPLib::GetCArray( uint32_t pid, tPrimitive( &srcCArray )[tSizeIn], 
                        tPrimitive( &dstCArray )[tSizeOut] )    // (6) C-Array

template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
void BSPLib::GetCArray( uint32_t pid, 
                        tPrimitive( &cArray )[tSizeIn] )        // (7) Same C-Array

template < typename tContainerIn, typename tContainerOut >
void BSPLib::GetContainer( uint32_t pid, tContainerIn &srcContainer,
                           tContainerOut &dstContainer )        // (8) General container   

template < typename tContainerIn, typename tContainerOut >
void BSPLib::GetContainer( uint32_t pid,
                           tContainer &srcContainer )           // (9) Same container
```

This operation get the registered memory of another processor and copies the 
memory into its own memory. Gets the values from the container in the processor 
with identifier `pid` and stores it the required destination container. 
Internally, it calculates the size in bytes that is required for the primitive, 
and calls to [`BSPLib::Classic::Get()`](get.md).


1. Gets a buffer of size `count` primitives from source iterator 
   `srcBegin` that is located in the processor with identifier `pid` at 
   offset `offset` primitives from source iterator `srcBegin` and stores 
   it at the location of `dstBegin`.
2. Has the same behaviour as (1), but now we use a cursor
   instead of a count. The offset is defined as `offset = srcCursor - srcBegin` 
   and `count = resultEnd - resultBegin`.
3. Has the same behaviour as (1), using that `srcBegin = begin`, 
   `srcCursor = resultBegin = cursor` and `resultEnd = end`.
4. Has the same behaviour as (3), using that `cursor = begin`.
5. Has the same behaviour as (1), using that `srcBegin = resultBegin = begin`.
6. Gets an entire C-Array as buffer from `srcCArray` located in the processor 
   with identifier `pid` and stores it at the location of `dstCArray`.
7. Has the same behaviour as (6), but now `srcCArray = dstCArray = cArray`, 
   so the source is in the same location as the destination, but in the 
   other processor.
8. Gets an entire container as buffer from `srcContainer` located in the 
   processor with identifier `pid` and stores it at the location of `dstContainer`.
9. Has the same behaviour as (8), but now `srcContainer = dstContainer = container`.

!!! danger "Warnings"
     * In case of (8) and (9), make sure the container is described by contiguous 
       iterators.

!!! tip
    There are also other specialisations available. See
    [Containers](getContainer.md), [Primitives](getPrimitive.md)
    and [Pointers](getPtrs.md).

#Parameters

* `pid` The identifier of the processor to communicate with.
* `srcBegin` Iterator describing the begin of the information in the other processor.
* `offset` Offset from the source `srcBegin` in number of primitives to start 
  reading from.
* `resultBegin` Iterator describing the destination for the information in 
  the current processor.
* `count` Number of primitives to read.
* `srcCursor` Other method to determine `offset`. The behaviour is the same as 
  taking `offset = srcCursor - srcBegin`.
* `resultEnd` Other method to determine `count`. The behaviour is the same as
  taking `count = resultEnd - resultBegin`.
* `begin` Iterator describing the begin of the information in both the processors.
* `cursor` Cursor to start reading from in the other processor, and to start writing
  to in the current processor.
* `end` End of the information block to get.
* `srcCArray` C-Array located in the other processor, the source of the information.
* `dstCArray` C-Array located in the current processor, the destination for 
  the information.
* `cArray` C-Array that is located in both the processors, both the source 
  and the destination for the information.
* `srcContainer` Container located in the other processor, the source of 
  the information.
* `dstContainer` Container located in the current processor, the destination 
  for the information.
* `container` Container that is located in both the processors, both the source 
  and the destination for the information.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on the source,
  in the processor with identifier `pid`, with at least the same size as the destination.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between 
  [`BSPLig::Push()`](../regdereg/push.md) and this call.

#Post-Conditions
* Get request has been queued.
* In the next superstep [`BSPLib::Sync()`](../sync/sync.md), the destination 
  will have the copied value from the source.
  
#Examples

### (1) Begin-Offset-Count

### (2) Begin-Cursor-End

### (3) Same Begin-Cursor-End

### (4) Same Begin-End

### (5) Same Begin-Offset-Count

### (6) C-Array

### (7) Same C-Array

### (8) General container

### (9) Same container