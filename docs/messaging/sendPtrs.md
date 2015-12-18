#Interfaces

```cpp
template< typename tPrimitive, typename tTagPrimitive >
void BSPLib::SendPtrs( uint32_t pid, tTagPrimitive *tag, tPrimitive *begin,
                       size_t count )                               // (1) Begin-Count-Pointer

template< typename tPrimitive, typename tTagPrimitive >
void BSPLib::SendPtrs( uint32_t pid, tTagPrimitive *tag, tPrimitive *begin,
                       tPrimitive *end )                            // (2) Begin-End-Pointer

template< typename tPrimitive, typename tTag >
void BSPLib::SendPtrs( uint32_t pid, const tTag &tag, tPrimitive *begin,
                       size_t count )                               // (3) Begin-Count-Reference

template< typename tPrimitive, typename tTag >
void BSPLib::SendPtrs( uint32_t pid, const tTag &tag, tPrimitive *begin,
                       tPrimitive *end )                            // (4) Begin-End-Reference

template< typename tPrimitive >
void BSPLib::SendPtrs( uint32_t pid, const std::string &tag, tPrimitive *begin,
                       tPrimitive *end )                            // (5) Begin-End-std::string

template< typename tPrimitive >
void BSPLib::SendPtrs( uint32_t pid, const std::string &tag, tPrimitive *begin,
                       size_t count )                               // (6) Begin-Count-std::string

template< typename tPrimitive >
void BSPLib::SendPtrs( uint32_t pid, tPrimitive *begin, 
                       size_t count )                               // (7) Begin-Count

template< typename tPrimitive >
void BSPLib::SendPtrs( uint32_t pid, tPrimitive *begin,
                       tPrimitive *end )                            // (8) Begin-End
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
* `begin` Pointer to the begin of the payload.
* `end` Pointer to the end of the payload.
* `count` Number of primitives in the payload.
* `tagPtr` Pointer to the beginning of the tag.
* `tagCArray` C-Array containing the tag.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* Tagsize is equal on all threads.
  
#Examples

###(1) Begin-Count-Reference
###(2) Begin-End-Pointer
###(3) Begin-Count-Pointer
###(4) Begin-End-Reference
###(5) Begin-Count-std::string
###(6) Begin-End-std::string
###(7) Begin-Count
###(8) Begin-End