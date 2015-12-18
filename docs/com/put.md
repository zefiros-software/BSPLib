#Interfaces

```cpp
void BSPLib::Classic::Put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset,
                           size_t nbytes )                                  // (1) Classic
void bsp_get( uint32_t pid, const void *src, void *dst, ptrdiff_t offset,
              size_t nbytes )                                               // (2) Legacy
```

Puts a buffer of size `nbytes` from source pointer `src` in the thread 
with identifier `pid` at offset `offset` from destination pointer `dst`.

1. Classic BSP function, this is the interface one should prefer to 
   use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards 
   compatibility with other BSP libraries.

!!! tip
    There are easier functions to work with. See for
    [Containers](putContainer.md), [Primitives](putPrimitive.md)
    and [Pointers](putPtrs.md).

#Parameters

* `pid` The ID of the processor to communicate with.
* `src` Pointer to the source of the information in the current processor.
* `offset` Offset from the destination `dst` in bytes to start writing from.
* `dst` Pointer to the destination for the information in the other processor.
* `nbytes` Number of bytes to write.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* `src != nullptr`.
* `dst != nullptr`.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on `dst` 
  with at leas size `offset + nbytes` in the processor with identifier `pid`.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between 
  [`BSPLig::Push()`](../regdereg/push.md) and this call.

#Post-Conditions
* Put request has been queued.
* In the next superstep [`BSPLib::Sync()`](../sync/sync.md), the destination 
  will have the copied value from the source.
     
#Examples

###(1) Classic

###(2) Legacy