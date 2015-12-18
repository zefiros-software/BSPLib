#Interfaces

```cpp
template< typename tTag, typename tContainer >
void BSPLib::SendContainer( uint32_t pid, tTag *tag,
                            tContainer &container )                     // (1) Container-Pointer

template< typename tTag, typename tContainer >
void BSPLib::SendContainer( uint32_t pid, tTag &tag,
                            tContainer &container )                     // (2) Container-Reference

template< typename tTagPrimitive, typename tContainer, size_t tTagSize >
void BSPLib::SendContainerWithCArray( uint32_t pid, tTagPrimitive( &tag )[tTagSize],
                                      tContainer &container )           // (3) Container-CArray

template< typename tContainer >
void BSPLib::SendContainer( uint32_t pid, tContainer &container )       // (4) Container

template< typename tPrimitive, typename tTag, size_t tSize >
void BSPLib::SendCArray( uint32_t pid, tTag *tag,
                         tPrimitive( &payload )[tSize] )                // (5) CArray-Pointer

template< typename tPrimitive, typename tTag, size_t tSize >
void BSPLib::SendCArray( uint32_t pid, tTag &tag,
                         tPrimitive( &payload )[tSize] )                // (6) CArray-Reference

template< typename tPrimitive, typename tTagPrimitive, size_t tSize, size_t tTagSize >
void BSPLib::SendCArrayWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize],
                                   tPrimitive( &payload )[tSize] )      // (7) CArray-CArray

template< typename tPrimitive, size_t tSize >
void BSPLib::SendCArray( uint32_t pid, tPrimitive( &payload )[tSize] )  // (8) CArray
```

Send a buffered message to the processor with ID pid using a tag to identify the message.

1. Sends an entire container as message, accompanied by a tag that may either be a primitive or a contiguous container.
2. Sends an entire container as message, accompanied by a tag that starts at `tagPtr`.
3. Sends an entire container as message, accompanied by a tag that is an entire C-Array.
4. Has the same behaviour as (2), using that `tagPtr = nullptr`.
5. Sends an entire C-Array as message, accompanied by a tag that may either be a primitive or a contiguous container.
6. Sends an entire C-Array as message, accompanied by a tag that starts at `tagPtr`.
7. Sends an entire C-Array as message, accompanied by a tag that is an entire C-Array.
8. Has the same behaviour as (6), using that `tagPtr = nullptr`.

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

###(1) Container-Pointer
###(2) Container-Reference
###(3) Container-CArray
###(4) Container
###(5) CArray-Pointer
###(6) CArray-Reference
###(7) CArray-CArray
###(8) CArray