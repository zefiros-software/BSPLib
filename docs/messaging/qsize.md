#Interfaces

```cpp
void BSPLib::Classic::QSize( size_t *packets, size_t *accumulatedSize ) // (1) Classic
void bsp_begin( size_t *packets, size_t *accumulatedSize )              // (2) Legacy

void BSPLib::QSize( size_t &packetsRef )                                // (3) Reference
void BSPLib::QSize( size_t &packetsRef, size_t &accumulatedSizeRef )    // (4) References
```

Get both the amount of messages, and the total size of the messages in bytes.

1. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.
3. Gets the amount of messages in the queue using a reference, as we do not allow `packets` to be `nullptr`.
4. Gets both the amount of messages in the queue and the total size of the messages in bytes, using references.
    
#Parameters

* `packets` Pointer to the location where the total number of packets is to be written.
* `accumulatedSize` Pointer to the location where the total size of the messages is to be written.
* `packetsRef` Reference to write the number of packets to.
* `accumulatedSizeRef` Reference to write the total size of the messages to.

#Pre-Conditions
* [`BSPLib::Begin()`](../logic/begin.md) has been called.
* packets != nullptr.

#Post-Conditions
* If `accumulatedSize != nullptr`:
    * accumulatedSize will be the accumulated size of the packets in bytes.
    
    Else:
    
    * The calculation of the accumulated size will be skipped.
  
#Examples

###(1) Classic

###(2) Legacy

###(3) Reference

###(4) References