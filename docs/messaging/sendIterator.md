#Interfaces

```cpp
template< typename tIterator, typename tTag >
void BSPLib::SendIterator( uint32_t pid, tTag &tag, tIterator begin, 
                           size_t count )                               // (1) Begin-Count-Reference

template< typename tIterator, typename tTag >
void BSPLib::SendIterator( uint32_t pid, tTag *tagPtr, tIterator begin, 
                           size_t count )                               // (2) Begin-Count-Pointer

template< typename tIterator, typename tTag >
void BSPLib::SendIterator( uint32_t pid, tTag &tag, tIterator begin,
                           tIterator end )                              // (3) Begin-End-Reference

template< typename tIterator, typename tTag >
void BSPLib::SendIterator( uint32_t pid, tTag *tagPtr, tIterator begin, 
                           tIterator end )                              // (4) Begin-End-Pointer

template< typename tIterator, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendIteratorWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize],
                                     tIterator begin, size_t count )    // (5) Begin-Count-CArray

template< typename tIterator, typename tTagPrimitive, uint32_t tTagSize >
void BSPLib::SendIteratorWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize], 
                                     tIterator begin, tIterator end )   // (6) Begin-End-CArray
                                    


template< typename tIterator >
void BSPLib::SendIterator( uint32_t pid, tIterator begin,
                           size_t count )                               // (7) Begin-Count

template< typename tIterator >
void BSPLib::SendIterator( uint32_t pid, tIterator begin,
                           tIterator end )                              // (8) Begin-End
```

Send a buffered message to the processor with ID pid using a tag to identify the message.

1. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that may either be a primitive or a contiguous container.
2. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that starts at `tagPtr`.
3. Has the same behaviour as (1), using that `count = end - begin`.
4. Has the same behaviour as (2), using that `count = end - begin`.
5. Sends a block of `count` primitives starting at `begin` as message, accompanied by a tag that is an entire C-Array.
6. Has the same behaviour as (5), using that `count = end - begin`.
7. Has the same behaviour as (2), using that `tagPtr = nullptr`, no tag is sent.
8. Has the same behaviour as (4), using that `tagPtr = nullptr`, no tag is sent.

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
* `tag` The tag by which the other processor can identify the message. This may be either a primitive or a contiguous container.
* `begin` Iterator describing the begin of the payload.
* `end` Iterator describing the end of the payload.
* `count` Number of primitives in the payload.
* `tagPtr` Pointer to the beginning of the tag.
* `tagContainer` C-Array containing the tag.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* Tagsize is equal on all threads.
  
#Examples

###(1) Begin-Count-Reference
###(2) Begin-Count-Pointer
###(3) Begin-End-Reference
###(4) Begin-End-Pointer
###(5) Begin-Count-CArray
###(6) Begin-End-CArray
###(7) Begin-Count
###(8) Begin-End