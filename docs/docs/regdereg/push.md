#Interfaces

```cpp
void BSPLib::Push( const void *ident, size_t byteSize )         // (1) Modern
void BSPLib::Push()                                             // (2) Padding

template< typename tPrimitive >
void BSPLib::Push( tPrimitive &identRef )                       // (3) Reference

template<>
void BSPLib::Push( std::string &stringRef )                     // (4) std::string

template< typename tPrimitive >
void BSPLib::PushPtrs( tPrimitive *begin, size_t count )        // (5) Primitive pointer

template< typename tPrimitive >
void BSPLib::PushPtrs( tPrimitive *begin, tPrimitive *end )     // (6) Primitive pointers

template < typename tIterator>
void BSPLib::PushIterator( tIterator beginIt, size_t count )    // (7) Iterator

template < typename tIterator>
void BSPLib::PushIterator( tIterator beginIt, tIterator endIt ) // (8) Iterators

template< typename tPrimitive, size_t tSize >
void BSPLib::PushCArray( tPrimitive( &cArray )[tSize] )        // (9) C-Array

template< typename tContainer >
void BSPLib::PushContainer( tContainer &container )            // (10) Container


void BSPLib::Classic::Push( const void *ident, size_t byteSize ) // (11) Classic
void bsp_pushreg( const void *ident, size_t byteSize )           // (12) BSP
```

Pushes a register, with the given size. BSPLib can have communication with any 
contiguous data type. To ensure this requirement, the library needs to know what
addresses are available for communication. To recognise addresses of other
processors, the order of [registration](push.md) and [deregistration](pop.md) 
needs to be the same in each processor. When one processor does not need to register
an address, a `nullptr`can be pushed, or the specialised function (2) can be used.
Each push should be accompanied by a pop, or is assumed to be popped at the end of
the BSP program.

1. Modern interface of the classic BSP function.
2. Adds padding if the current processor does not need to push.
3. Pushes a primitive as register. Computes the bytesize internally.
4. Template specialisation for `std::string`.
5. Pushes the address of `begin`, with bytesize `count * sizeof(tPrimitive)`.
6. Pushes all addresses from `begin` to `end`. Computes the bytesize internally.
7. Pushes the address of the `beginIt` iterator up to `count` adresses more.
8. Pushes all addresses from `beginIt` to `endIt`. Computes the bytesize internally.
9. Pushes all addresses of the C-Array, with bytesize `tSize * sizeof(tPrimitive)`.
10. Pushes all addresses of the container, with bytesize `container.size() * sizeof(tContainer::value_type)`.
11. Classic BSP function.
12. Legacy BSP function.

!!! danger "Warnings"
     * In case of (4), the std::string must not be resized. If a resize is absolutely
       necessary, it should be done before pushing the register, or after popping the
       register. Also, make sure the processes agree on the size beforehand.
     * In case of (7), (8) and (10), make sure the iterators describe contiguous memory, 
       and are not invalidated (due to resizes).
     * When `BSPLib::Push()` is called after `BSPLib::Pop()`, the register that has
       been popped will not be replaced by the new register, as the naming might
       suggest. 
       
!!! note "Notes"
    In the current implementation, the pointer to the top of the registration stack
    is never lowered. In the programs we tested until now, this has not been a
    problem. We might change this behaviour in the future. In case we do, no changes 
    will be needed in existing BSP programs. The stack is always cleared when a new
    BSP program is initialised.


#Parameters

* `ident` The address to register.
* `byteSize` The size in bytes of the address space.
* `identRef` Reference to the variable to register.
* `stringRef` Reference to the string to regiser.
* `begin` Begin of the address space to register.
* `count` Number of addresses to register in the current space.
* `end` End of the address space to register.
* `beginIt` Iterator describing the begin of the address space to register.
* `endIt` Iterator describing the end of the address space to register.
* `cArray` C-Array of which we register all addresses.
* `container` Container of which we register all addresses.

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