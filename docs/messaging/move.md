#Interfaces

```cpp
void BSPLib::Classic::Move( void *payload, size_t max_copy_size_in )    // (1) Classic
void bsp_move( void *payload, size_t max_copy_size_in )                 // (2) Legacy

template< typename tPrimitive >
void BSPLib::Move( tPrimitive &payloadRef )                             // (3) Reference

template<>
void BSPLib::Move( std::string &payloadRef )                            // (4) std::string

template< typename tPrimitive >
void BSPLib::MovePtrs( tPrimitive *begin, uint32_t maxCopySize )        // (5) Pointers: Begin-Count

template< typename tPrimitive >
void BSPLib::MovePtrs( tPrimitive *begin, tPrimitive *end )             // (6) Pointers: Begin-End

template< typename tIterator >
void BSPLib::MoveIterator( tIterator beginIt, uint32_t maxCopySize )    // (7) Iterators: Begin-Count

template< typename tIterator >
void BSPLib::MoveIterator( tIterator beginIt, tIterator endIt )         // (8) Iterators: Begin-End

template< typename tPrimitive, size_t tSize >
void BSPLib::MoveCArray( tPrimitive( &payloadCArray )[tSize] )          // (9) C-Array

template< typename tContainer >
void BSPLib::MoveContainer( tContainer &payloadContainer )              // (10) Container
```

Moves the first message in the queue to the given payload destination.

1. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.
3. Moves the first message in the queue to the given primitive.
4. Overload of (3) for `std::string`.
5. Moves the first message in the queue to `begin`, with up to `maxCopySize` primitives.
6. Has the same behaviour as (5), using that `maxCopySize = begin - end`.
7. Moves the first message in the queue to `beginIt`, with up to `maxCopySize` primitives.
8. Has the same behaviour as (5), using that `maxCopySize = beginIt - endIt`.
9. Moves the first message in the queue to the C-Array.
10. Moves the first message in the queue to the container.
    
#Parameters

* `payload` Void pointer to the begin of the payload destination.
* `max_copy_size_in` Maximum size in bytes for the payload destination. No more than this will be copied (may copy less).
* `payloadRef` Reference to store the payload to.
* `begin` Typesafe pointer to the begin of the payload destination.
* `maxCopySize` Maximum number of primitives for the payload destination. Similar to `max_copy_size_in`, but not in bytes.
* `end` Typesafe pointer to the end of the payload destination.
* `beginIt` Iterator describing the begin of the payload destination.
* `endIt` Iterator describing the end of the payload destination.
* `payloadCArray` C-Array that is the destination for the payload.
* `payloadContainer` Container that is the destination for the payload.

#Pre-Conditions
* Begin has been called.
* If the send queue is empty or the cursor is at/behind the end:
    * Does nothing.
    
    Else:
    
    * Moves the first message in the queue o the given payload destination.
* payload != nullptr.

#Post-Conditions
The queue cursor for the send queue is moved to the next message.
  
#Examples

###(1) Classic
###(2) Legacy
###(3) Reference
###(4) std::string
###(5) Pointers: Begin-Count
###(6) Pointers: Begin-End
###(7) Iterators: Begin-Count
###(8) Iterators: Begin-End
###(9) C-Array
###(10) Container