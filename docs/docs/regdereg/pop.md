#Interfaces

```cpp
void BSPLib::Pop()			  // (1) Modern
void BSPLib::Classic::Pop()   // (2) Classic
void bsp_popreg()			  // (3) BSP
```

Pops the register described by `ident`.

#Parameters

* `ident` The buffer pointer to register.

#Pre-Conditions
* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called.

#Post-Conditions
* Pop request has been queued.
* In the next superstep, this register will be unavailable for [`BSPLib::Put()`](../com/put.md)/[`BSPLib::Get()`](../com/get.md).
	 
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Classic

This is the implementation of the classic interface, in a modern style.

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.