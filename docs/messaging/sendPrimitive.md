#Interfaces

```cpp
template< typename tPrimitive, typename tTag >
void BSPLib::Send( uint32_t pid, tTag &tag,
                   const tPrimitive &payload )                      // (1) References

template< typename tTag >
void BSPLib::Send( uint32_t pid, tTag &tag, 
                   const std::string &payload )                     // (2) std::string-Reference

template< typename tPrimitive, typename tTag >
void BSPLib::Send( uint32_t pid, tTag *tagPtr, 
                   const tPrimitive &payload )                      // (3) Reference-Pointer

template< typename tTag >
void BSPLib::Send( uint32_t pid, tTag *tagPtr,
                   const std::string &payload )                     // (4) std::string-Pointer

template< typename tPrimitive >
void BSPLib::Send( uint32_t pid, const tPrimitive &payload )        // (5) Reference

template<>
void BSPLib::Send( uint32_t pid, const std::string &payload )       // (6) std::string

template< typename tPrimitive, typename tTagPrimitive, size_t tTagSize >
void BSPLib::SendWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize],
                     const tPrimitive &payload )                    // (7) Reference-CArray

template< typename tTagPrimitive, size_t tTagSize >
void BSPLib::SendWithCArray( uint32_t pid, tTagPrimitive( &tagContainer )[tTagSize],
                     const std::string &payload )                   // (8) std::string-CArray
```

Send a buffered message to the processor with ID pid using a tag to identify the message.

1. Sends one primitive as message, accompanied by a tag that may either be a primitive or a contiguous container.
2. Overload of (1) for `std::string`.
3. Sends one primitive as message, accompanied by a tag that starts at `tagPtr`.
4. Overload of (3) for `std::string`.
5. Sends one primitive as message, without a tag.
6. Overload of (5) for `std::string`.
7. Sends one primitive as message, accompanied by a tag that is a C-Array.
8. Overload of (7) for `std::string`.

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
* `payload` Reference to the primitive that will be the payload of the message.
* `tagPtr` Pointer to the beginning of the tag.
* `tagContainer` C-Array containing the tag.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* Tagsize is equal on all threads.
  
#Examples

###(1) References
###(2) std::string-Reference
###(3) Reference-Pointer
###(4) std::string-Pointer
###(5) Reference
###(6) std::string
###(7) Reference-CArray
###(8) std::string-CArray