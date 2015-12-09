#Interfaces

```cpp
template< typename tPrimitive, typename tTagPrimitive >
void BSPLib::SendPtrs( uint32_t pid, std::vector< tTagPrimitive > &tagVector, tPrimitive *begin, 
                       size_t count )                                           // (1) Begin-Count-Vector

template< typename tPrimitive, typename tTagPrimitive >
void BSPLib::SendPtrs( uint32_t pid, std::vector< tTagPrimitive > &tagVector, tPrimitive *begin,
                       tPrimitive *end )                                        // (2) Begin-End-Vector

template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendPtrs( uint32_t pid, std::array< tTagPrimitive, tTagSize > &tagArray,
                       tPrimitive *begin, size_t count )                        // (3) Begin-Count-StdArray

template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendPtrs( uint32_t pid, std::array< tTagPrimitive, tTagSize > &tagArray,
                       tPrimitive *begin, tPrimitive *end )                     // (4) Begin-End-StdArray

template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendPtrsWithCArray( uint32_t pid, tTagPrimitive( &tagCArray )[tTagSize],
                                 tPrimitive *begin, size_t count )              // (5) Begin-Count-CArray

template< typename tPrimitive, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendPtrsWithCArray( uint32_t pid, tTagPrimitive( &tagCArray )[tTagSize],
                                 tPrimitive *begin, tPrimitive *end )           // (6) Begin-End-CArray
```

Send a buffered message to the processor with ID pid using a tag to identify the message.

1. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that consists of an entire `std::vector`.
2. Has the same behaviour as (1), using that `count = end - begin`.
3. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that consists of an entire `std::array`.
4. Has the same behaviour as (3), using that `count = end - begin`.
5. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that consists of an entire C-Array.
6. Has the same behaviour as (5), using that `count = end - begin`.

!!! tip
    * There are also other specialisations available. See
      [Containers](sendContainer.md), [Primitives](sendPrimitive.md), [Iterators](sendIterator.md),
      [Pointes with Primitive Tags](sendPtrs.md) and [Pointers with Container Tags](sendPtrs.md).
    * You can safely pass both primitives and contiguous containers, like `std::vector`
      or `std:array` using the `tag` parameter. They will be specialised by 
      [`BSPLib::SendPtrs()`](sendPtrs.md) that is called internally.
    * If you want to use a reference to a primitive as a tag, have a look at
      [Send Memory - Pointers - Primitives](sendPtrs.md).
    * If you want to use a reference to a container as a tag, have a look at
      [Send Memory - Pointers - Containers](sendTagContainer.md).

!!! danger "Warnings"
    Make sure that in all cases, the tagsize is appropriately set. Using these functions does not guarantee that the tagsize
    is always correct. See [`BSPLib::SetTagSize()`](../messagingutil/settagsize.md).
    
#Parameters

* `pid` The ID of the processor to send to.
* `begin` Iterator describing the begin of the payload.
* `end` Iterator describing the end of the payload.
* `count` Number of primitives in the payload.
* `tagVector` `std::vector` containing the entire tag.
* `tagArray` `std::array` containing the entire tag.
* `tagCArray` C-Array containing the entire tag.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* Tagsize is equal on all threads.
  
#Examples

###(1) Begin-Count-Vector
###(2) Begin-End-Vector
###(3) Begin-Count-StdArray
###(4) Begin-End-StdArray
###(5) Begin-Count-CArray
###(6) Begin-End-CArray