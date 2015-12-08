#Interfaces

```cpp
void BSPLib::Classic::GetTag( size_t *status, void *tag )               // (1) Classic
void bsp_get_tag( size_t *status, void *tag )                           // (2) Legacy

template< typename tPrimitive >
void BSPLib::GetTag( size_t &statusRef, tPrimitive &tagRef )            // (3) Reference

template<>
void BSPLib::GetTag( size_t &statusRef, std::string &tagRef )           // (4) std::string

template< typename tPrimitive >
void BSPLib::GetTagPtr( size_t &statusRef, tPrimitive *tagPtr )              // (5) Pointer

template< typename tIterator >
void BSPLib::GetTagIterator( size_t &statusRef, tIterator tagBegin )         // (6) Iterator

template< typename tContainer >
void BSPLib::GetTagContainer( size_t &statusRef, tContainer &tagContainer )  // (7) Container

template< typename tPrimitive, size_t tSize >
void BSPLib::GetTagCArray( size_t &statusRef, tPrimitive( &tagCArray )[tSize] ) // (8) C-Array
```

Gets the tag for the next message in the send queue.

1. Classic BSP function.
2. Legacy BSP function
3. Uses a reference for `status`, as it is not allowed to be `nullptr`. Uses a reference to store the tag.
4. Specialisation of (3) for `std::string`.
5. Uses a reference for `status`. More typesafe version for the tag pointer.
6. Uses a reference for `status`. Uses an iterator to describe te beginning of the tag.
7. Uses a reference for `status`. Uses a container to store the tag.
8. Uses a reference for `status`. Uses a C-Array to store the tag. 

#Parameters

* `status` Void pointer to store the status to.
* `tag` Void pointer to store the tag to.
* `statusRef` Reference to store the status to.
* `tagRef` Reference to store the tag to.
* `tagPtr` Typesafe pointer to store the tag to.
* `tagBegin` Iterator describing the begin of the tag.
* `tagContainer` Container to store the tag to.
* `tagCArray` C-Array to store the tag to.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* If the send queue is empty or the cursor is at/behind the end of the queue:
    * Returns a status equal to `-1`.
    
    Else:
    
    * Returns the size of the buffer of the request as status.
    * Writes the tag to the desired destination.

#Post-Conditions
The queue is in the same state as before.
     
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.