#Interfaces

```cpp
template< typename tPrimitive >
void BSPLib::Get( uint32_t pid, tPrimitive &src, tPrimitive &dst )      // (1) References

template<>
void BSPLib::Get( uint32_t pid, std::string &src, std::string &dst )    // (2) std::string

template< typename tPrimitive >
void BSPLib::Get( uint32_t pid, tPrimitive &var )                       // (3) Reference
```

Gets the primitive stored in `src` that is located in the processor with ID `pid` and stores it at the location of `dst`. 
Internally, it calculates the size in bytes that is required for the primitive, and calls to [`BSPLib::Classic::Get()`](get.md).

1. Gets the value from `src` in the processor with ID `pid` and stores it in `dst`, using references.
2. Overload for `std::string`.
3. Get the value from `var` in the processor with ID `pid` and stores it in `var`, the same reference.

!!! danger "Warnings"
     * In case of (2), the destination `std::string` must be at least as big as the source `std::string`. 
       Make sure it is correctly resized, either by deciding on a fixed size, or communicating the size beforehand.


#Parameters

* `pid` The ID of the processor to communicate with.
* `srcRef` Reference to the source of the information in the other processor.
* `dstRef` Reference to the destination for the information in the current processor.
* `var` Reference to both the source of the in the other processor, and the destination for the information in the
        current processor.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on `src` in the processor with ID `pid`.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between [`BSPLig::Push()`](../regdereg/push.md) and this call.
* In case of (2), the `dst` string is at least as big as the `src` string.

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