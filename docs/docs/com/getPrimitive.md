#Interfaces

```cpp
template< typename tPrimitive >
void BSPLib::Get( uint32_t pid, tPrimitive &src, tPrimitive &dst )      // (1) References

template<>
void BSPLib::Get( uint32_t pid, std::string &src, std::string &dst )    // (2) std::string references

template< typename tPrimitive >
void BSPLib::Get( uint32_t pid, tPrimitive &var )                       // (3) Reference
```

Pushes a register, with the given size. 

1. Modern interface of the classic BSP function.

!!! danger "Warnings"
     * In case of (4), the std::string must not be resized.


#Parameters

* `ident` The address to register.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* Push request has been queued.
* In the next superstep, this register will be available for [`BSPLib::Put()`](../com/put.md)/[`BSPLib::Get()`](../com/get.md).
     
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.