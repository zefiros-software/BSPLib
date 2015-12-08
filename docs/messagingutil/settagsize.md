#Interfaces

```cpp
void BSPLib::Classic::SetTagSize( size_t *size )    // (1) Classic
void bsp_set_tagsize( size_t *size )                // (2) Legacy

template< typename tPrimitive >
void BSPLib::SetTagsize()                           // (3) Primitive

template< typename tPrimitive >
void SetTagsize( uint32_t count )                   // (4) Count
```

Sets a tagsize for the next superstep.

1. Classic BSP function.
2. Legacy BSP function
3. Sets the tagsize to the size of a primitive, `sizeof(tPrimitive)` 
   will be the new tagsize.
4. Sets the tagsize to the size of count primitives, `count * sizeof(tPrimitive)`.

#Parameters

* `size` Pointer to the desired size.
* `count` Number of primitives to set the tagsize to.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* `size != nullptr`.

#Post-Conditions
The tagsize will be updated during the next [synchronization](../sync/sync.md)
     
#Examples

### (1) Classic

### (2) Legacy

### (3) Primitive

### (4) Count