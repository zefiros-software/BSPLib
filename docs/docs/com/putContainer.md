#Interfaces

```cpp
template< typename tIterator, typename tOutputIterator >
    void BSPLib::PutIterator( uint32_t pid, tIterator srcBegin, size_t count, 
                      tOutputIterator resultBegin, 
                      size_t offset )                           // (1) Begin-Offset-Count

template< typename tIterator, typename tOutputIterator >
void BSPLib::PutIterator( uint32_t pid, tIterator srcBegin, tIterator srcEnd, 
                          tOutputIterator resultBegin,
                          tOutputIterator resultDst )           // (2) Begin-Cursor-End

template< typename tIterator >
void BSPLib::PutIterator( uint32_t pid, tIterator begin, 
                          tIterator cursor, tIterator end )     // (3) Same Begin-Cursor-End

template< typename tIterator >
void BSPLib::PutIterator( uint32_t pid, tIterator begin, 
                          tIterator end )                       // (4) Same Begin-End

template< typename tIterator >
void BSPLib::PutIterator( uint32_t pid, tIterator begin,
                          size_t offset, size_t count )         // (5) Same Begin-Offset-Count

template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
void BSPLib::PutCArray( uint32_t pid, tPrimitive( &srcCArray )[tSizeIn], 
                        tPrimitive( &dstCArray )[tSizeOut] )    // (6) C-Array

template< typename tPrimitive, size_t tSizeIn, size_t tSizeOut >
void BSPLib::PutCArray( uint32_t pid, 
                        tPrimitive( &cArray )[tSizeIn] )        // (7) Same C-Array

template < typename tContainerIn, typename tContainerOut >
void BSPLib::PutContainer( uint32_t pid, tContainerIn &srcContainer,
                           tContainerOut &dstContainer )        // (8) General container 

template < typename tContainerIn, typename tContainerOut >
void BSPLib::PutContainer( uint32_t pid
                           tContainer &container )              // (9) Same container
```

Pushes a register, with the given size. 

1. Puts a buffer of size `count` primitives from source iterator 
   `srcBegin` that is located in the current processor
   and stores it at the location of `dstBegin` in the processor with identifier 
   `pid`, at offset `offset` from `dstBegin`.
2. Has the same behaviour as (1), using that `offset = resultDst - resultBegin` 
   and `count = srcEnd - srcBegin`.
3. Has the same behaviour as (1), using that `resultBegin = begin`, 
   `srcBegin = resultDst = cursor` and `srcEnd = end`.
4. Has the same behaviour as (3), using that `cursor = begin`.
5. Has the same behaviour as (1), using that `srcBegin = resultBegin = begin`.
6. Puts an entire C-Array as buffer from `srcCArray` and stores it at the 
   location of `dstCArray`in the processor with identifier `pid`.
7. Has the same behaviour as (6), but now `srcCArray = dstCArray = cArray`, 
   so the source is in the same location as the destination, but in the other processor.
8. Puts an entire container as buffer from `srcContainer` and stores it 
   at the location of `dstContainer` located in the processor with identifier `pid` .
9. Has the same behaviour as (8), but now `srcContainer = dstContainer = container`.

!!! tip
    There are also other specialisations available. See
    [Containers](putContainer.md), [Primitives](putPrimitive.md)
    and [Pointers](putPtrs.md).
    
!!! danger "Warnings"
     * In case of (8) and (9), make sure the container is described by 
       contiguous iterators.

#Parameters

* `pid` The identifier of the processor to communicate with.
* `srcBegin` Iterator describing the begin of the information in the 
  current processor.
* `offset` Offset from the source `srcBegin` in number of primitives to 
  start reading from.
* `resultBegin` Iterator describing the destination for the information in 
  the other processor.
* `count` Number of primitives to read.
* `srcEnd` Other method to determine `count`. The behaviour is the same as
  taking `count = srcEnd - srcBegin`.
* `resultDst` Other method to determine `offset`. The behaviour is the same 
  as taking `offset = resultDst - resultBegin`.
* `begin` Iterator describing the begin of the information in both the processors.
* `cursor` Cursor to start reading from in the current processor, and to 
  start writing to in the other processor.
* `end` End of the information block to put.
* `srcCArray` C-Array located in the current processor, the source of the 
  information.
* `dstCArray` C-Array located in the other processor, the destination for
  the information.
* `cArray` C-Array that is located in both the processors, both the source
  and the destination for the information.
* `srcContainer` Container located in the current processor, the source
  of the information.
* `dstContainer` Container located in the other processor, the destination
  for the information.
* `container` Container that is located in both the processors, both the
  source and the destination for the information.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on the destination, 
  in the processor with identifier `pid`, with at least the same size as the source.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between 
  [`BSPLig::Push()`](../regdereg/push.md) and this call.

#Post-Conditions
* Put request has been queued.
* In the next superstep [`BSPLib::Sync()`](../sync/sync.md), destination in the 
  current processor will have the copied value from the source in the other processor.
     
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