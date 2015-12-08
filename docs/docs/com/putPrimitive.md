#Interfaces

```cpp
template< typename tPrimitive >
void BSPLib::Put( uint32_t pid, tPrimitive &src, tPrimitive &dst )      // (1) References

template<>
void BSPLib::Put( uint32_t pid, std::string &src, std::string &dst )    // (2) std::string

template< typename tPrimitive >
void BSPLib::Put( uint32_t pid, tPrimitive &var )                       // (3) Reference
```

Puts the primitive stored in `src` that is located in the current processor
and stores it at the location of `dst` that is located in the processor with ID `pid`. 
Internally, it calculates the size in bytes that is required for the primitive, and calls to [`BSPLib::Classic::Put()`](put.md).

1. Puts the value from `src` and stores it in `dst` in the processor with ID `pid`, using references.
2. Overload for `std::string`.
3. Put the value from `var` and stores it in `var`in the processor with ID `pid`, the same reference.

!!! danger "Warnings"
     * In case of (2), the destination `std::string` must be at least as big as the source `std::string`. 
       Make sure it is correctly resized, either by deciding on a fixed size, or communicating the size beforehand.


#Parameters

* `pid` The ID of the processor to communicate with.
* `src` Reference to the source of the information in the current processor.
* `dst` Reference to the destination for the information in the other processor.
* `var` Reference to both the source of the in the current processor, and the destination for the information in the
        other processor.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* [`BSPLib::Push()`](../regdereg/push.md) has been called on `dst` in the processor with ID `pid`.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between [`BSPLig::Push()`](../regdereg/push.md) and this call.
* In case of (2), the `dst` string is at least as big as the `src` string.

#Post-Conditions
* Put request has been queued.
* In the next superstep, the destination will have the copied value from the source.
     
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.