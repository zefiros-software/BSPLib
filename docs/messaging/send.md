#Interfaces

```cpp
void BSPLib::Classic::Send( uint32_t pid, const void *tag,
                            const void *payload, size_t size ) // (1) Classic
void bsp_send( uint32_t pid, const void *tag,
               const void *payload, size_t size )              // (2) Legacy
```

Send a buffered message to the processor with ID pid using a tag to identify the message.

1. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.

!!! tip    
    There are easier functions to work with. See for [Containers](sendContainer.md),
    [Primitives](sendPrimitive.md), [Iterators](sendIterator.md), [Pointers with Primitive Tags](sendPtrs.md)
    and [Pointers with Container Tags](sendPtrs.md).
    
#Parameters

* `pid` The ID of the processor to send to.
* `tag` The tag by which the other processor can identify the message.
* `payload` Pointer to the payload of the message.
* `size` Size of the message in bytes.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* Tagsize is equal on all threads.
  
#Examples

###(1) Classic

###(2) Legacy