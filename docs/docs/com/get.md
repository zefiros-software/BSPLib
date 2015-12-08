#Interfaces

```cpp
void BSPLib::Classic::Get( uint32_t pid, const void *src, ptrdiff_t offset,
                           void *dst, size_t nbytes )                       // (1) Classic
void bsp_get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst,   
              size_t nbytes )                                               // (2) Legacy
```

Gets a buffer of size `nbytes` from source pointer `src` that is located in the processor with ID `pid` at offset `offset` 
bytes from source pointer `src` and stores it at the location of `dst`.

1. Classic BSP function.
2. Legacy BSP function

#Parameters

* `pid` The ID of the processor to communicate with.
* `src` Pointer to the source of the information in the other processor.
* `offset` Offset from the source `src` in bytes to start reading from.
* `dst` Pointer to the destination for the information in the current processor.
* `nbytes` Number of bytes to read.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* `src != nullptr`.
* `dst != nullptr`.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on `src` with at least size `offset + nbytes` in the processor with ID `pid`.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between [`BSPLig::Push()`](../regdereg/push.md) and this call.

#Post-Conditions
* Get request has been queued.
* In the next superstep, the destination will have the copied value from the source.
     
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.