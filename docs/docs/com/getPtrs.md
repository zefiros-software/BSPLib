#Interfaces

```cpp
template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcCursor, 
              tPrimitive *resultBegin, tPrimitive *resultEnd )          // (4) Pointers

template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *srcBegin, size_t offset,
              tPrimitive *resultBegin, size_t count )                       // (1) 

template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *cursor,
              tPrimitive *end )                                             // (2)

template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *begin, tPrimitive *end )    // (3)

template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *begin, size_t offset,
                      size_t count )                                        // (4)

template< typename tPrimitive >
void BSPLib::GetPtrs( uint32_t pid, tPrimitive *begin, size_t count )       // (5)
```

Retrieves a buffer of size `count`, starting at `begin`

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