#Interfaces

```cpp
void BSPLib::Pop( const void *ident )                       // (1) Modern
void BSPLib::Pop()                                          // (2) Padding

template< typename tPrimitive >
void BSPLib::Pop( tPrimitive &identRef )                    // (3) Reference

template<>
void BSPLib::Pop( std::string &stringRef )                  // (4) std::string

template< typename tPrimitive >
void BSPLib::PopPtrs( tPrimitive *begin )                   // (5) Primitive pointer

template < typename tIterator>
void BSPLib::PopIterator( tIterator beginIt )               // (6) Iterator

template< typename tPrimitive, size_t tSize >
void BSPLib::PopCArray( tPrimitive( &container )[tSize] )   // (7) C-Array

template< typename tContainer >
void BSPLib::PopContainer( tContainer &container )          // (8) Container

void BSPLib::Classic::Pop( const void *ident )              // (9) Classic
void bsp_popreg( const void *ident )                        // (10) BSP
```

Pops a register and makes it unavailable in the next superstep. 
BSPLib can have communication with any 
contiguous data type. To ensure this requirement, the library needs to know what
addresses are available for communication. To recognise addresses of other
processors, the order of [registration](push.md) and [deregistration](pop.md) 
needs to be the same in each processor. When one processor does not need to register
an address, a `nullptr`can be pushed, or the specialised function (2) can be used.
Each push should be accompanied by a pop, or is assumed to be popped at the end of
the BSP program.

1. Modern interface of the classic BSP function.
2. Removes padding added by [`BSPLib::Push()`](push.md) (without parameters).
3. Pops the register belonging to a primitive.
4. Template specialisation for `std::string`.
5. Pops the address of `begin`.
6. Pops the address of `beginIt`.
7. Pops the addresses of the C-Array.
8. Pops the addresses of the container.
9. Classic BSP function.
10. Legacy BSP function.

!!! danger "Warnings"
    * In case of (4), the `std::string` must not have been resized.
    * In case of (6) and (8), make sure the iterators describe cotiguous memory,
      and have not been invalidated (due to resizes).
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

* `ident` The buffer pointer to register.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* Pop request has been queued.
* In the next superstep, this register will be unavailable for [`BSPLib::Put()`](../com/put.md)/[`BSPLib::Get()`](../com/get.md).
     
#Examples

### (1) Modern

### (2) Padding

### (3) Reference

### (4) std::string

### (5) Primitive pointer

### (6) Iterator

### (7) C-Array

### (8) Container

### (9) Classic

### (10) BSP