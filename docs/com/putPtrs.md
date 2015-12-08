#Interfaces

```cpp
template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *srcBegin, size_t count, 
                      tPrimitive *resultBegin, size_t offset )      // (1) Begin-Offset-Count
              
template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *srcBegin, tPrimitive *srcEnd, 
                  tPrimitive *resultBegin, tPrimitive *resultDst )  // (2) Begin-End-Cursor

template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *begin,
                      tPrimitive *cursor, tPrimitive *end ) // (3) Same Begin-End-Cursor

template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *begin, 
                      tPrimitive *end )                     // (4) Same Begin-End

template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *begin,
                      size_t offset, size_t count )         // (5) Same Begin-Offset-Count

template< typename tPrimitive >
void BSPLib::PutPtrs( uint32_t pid, tPrimitive *begin,
                      size_t count )                        // (6) Same Begin-Count
```

Puts a buffer of size `count` primitives from source pointer `srcBegin` that is 
located in the current processor, and stores it at offset `offset` from the 
location of `dstBegin` in the processor with ID `pid`.

1. Puts a buffer of size `count` primitives from source pointer `srcBegin` that 
   is located in the current processor, 
   and stores it at offset `offset` from the location of `dstBegin` in the 
   processor with ID `pid`.
2. Uses `srcBegin` and `srcEnd` pointers to calculate the number of primitives 
   `count`, and uses `resultDst` instead of `offset`
   to determine the offset it needs to start reading from.
3. The same as (1), but now, it uses `begin` as `resultBegin`, `cursor` 
   as both `srcBegin` and `resultDst`, and `end` as `srcEnd`.
   It stores the values in the same location as they were located in the 
   other processor.
4. The same as (3), but now `begin` is used as both `begin` and `cursor`. 
   It copies the entire range from `begin` to `end` from the
   processor with ID `pid` to the same location in the current processor.
5. The same as (3), with the behaviour: `cursor = begin + offset` and 
   `end = begin + offset + count`.
6. The same as (5), with the behaviour: `offset = 0`.

!!! tip
    There are also other specialisations available. See
    [Containers](putContainer.md), [Primitives](putPrimitive.md)
    and [Pointers](putPtrs.md).
    
!!! danger "Warnings"
     * `std::string` is not assumed to be a primitive in this case, although it was in [`BSPLib::Put()`](putPrimitive.md).

#Parameters

* `pid` The ID of the processor to communicate with.
* `srcBegin` Pointer to the begin of the information in the other processor.
* `offset` Offset from the source `srcBegin` in number of primitives to start 
  reading from.
* `resultBegin` Pointer to the destination for the information in the current processor.
* `count` Number of primitives to read.
* `resultDst` Other method to determine `offset`. The behaviour is the same 
  as taking `offset = resultDst - resultBegin`.
* `srcEnd` Other method to determine `count`. The behaviour is the same 
  as taking `count = srcEnd - srcBegin`.
* `begin` Pointer to the begin of the information in both the processors.
* `cursor` Cursor to start reading from in the current processor, and 
  to start writing to in the other processor.
* `end` End of the information block to put.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* None of the pointers `srcBegin`, `resultBegin`, `resultDst`, `srcEnd`,
  `begin`, `cursor` and `end` is allowed to be `nullptr`. 
* [`BSPLib::Push()`](../regdereg/push.md) has been called on `resultBegin`
  or `begin` with at least size `offset + nbytes` in the processor with ID `pid`.
* A [`BSPLib::Sync()`](../sync/sync.md) has happened between 
  [`BSPLig::Push()`](../regdereg/push.md) and this call.

#Post-Conditions
* Put request has been queued.
* In the next superstep [`BSPLib::Sync()`](../sync/sync.md), the 
  destination in the current processor will have the copied value 
  from the source in the other processor.
     
#Examples

### (1) Begin-Offset-Count

### (2) Begin-End-Cursor

### (3) Same Begin-End-Cursor

### (4) Same Begin-End

### (5) Same Begin-Offset-Count

### (6) Same Begin-Count