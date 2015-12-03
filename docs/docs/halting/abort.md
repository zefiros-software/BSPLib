#Interfaces

```cpp
void BSPLib::Abort( const char *errorMessage, ... )                    // (1) Modern
void BSPLib::VAbort( const char *errorMessage, va_list args )          // (2) Modern 2
void BSPLib::Classic::Abort( const char *errorMessage, ... )           // (3) Classic
void BSPLib::Classic::VAbort( const char *errorMessage, va_list args ) // (4) Classic 2
void bsp_abort( const char *errorMessage, ... )                        // (5) BSP
void bsp_vabort( const char *errorMessage, va_list args )              // (6) BSP 2
```

Aborts the BSP program with the given error message. The formatting used is the same as in
fprintf. If more than one processors calls [`BSPLib::Abort()`](abort.md), than
all processors may print the abort message, and not just one. 

The thread that aborts prints the message to `stderr`.
Instead of terminating the whole program, it just terminates
the computation, and thus you are able to continue or restart
the program. When you use [`BSPLib::Execute()`](../logic/execute.md)
you can query whether the program was aborted or not. When  [`BSPLib::Execute()`](../logic/execute.md)
is not used, the main thread may throw an exception.
  
#Parameters

* `format` Describes the message to format.
* `...`    Variable arguments providing message formatting.
* `args`   The formatting arguments.

#Post-Conditions
* All threads other than main thread will exit on a BSP call, unless the BSP process is restarted.
* If main thread, a BSPAbort exception will be thrown.

#Warnings
* The termination behaviour is different in this BSP implementation
  than in other libraries.
  
#Examples

###(1) Modern

This is interface one should choose to use.

###(2) Modern 2

This is interface one should choose to use. This version however is slightly awkward.

###(3) Classic

This is the implementation of the classic interface, in a modern style.

###(4) Classic 2

This is the implementation of the classic interface, in a modern style. This version is slightly awkward.


###(5) BSP

This interface is included for backwards compatibility with other BSP libraries.

###(6) BSP 2

This interface is included for backwards compatibility with other BSP libraries. This version is slightly awkward.

