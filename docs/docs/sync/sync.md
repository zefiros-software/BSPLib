#Interfaces

```cpp
void BSPLib::Sync()			  // (1) Modern
void BSPLib::Classic::Sync()  // (2) Classic
void bsp_sync()				  // (3) BSP
```

Synchronises all threads and communications. This marks the end of a computation super step, 
and each processor can handle its own computations between the super steps. At the end and not earlier
of the super step in the synchronisation point, the communication is handled. The synchronisation 
point also ensures all threads are synchronised, and functions as a barrier.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* All threads have completed this superstep.
* Tagsize has been synchronized and is available in the next superstep.
* Get requests have been processed, updated data is available in the next superstep.
* Pop requests have been processed, registers will be unavailable in the next superstep.
* Put requests have been processed, updated data is available in the next superstep.
* Buffers for the put requests have been cleared.
* Push requests have been processed, registers will be available in the next superstep.
	 
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.