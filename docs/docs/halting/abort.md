#Interfaces

```cpp
void BSPLib::Classic::Abort( const char *errorMessage, ... )           // (1) Classic
void bsp_abort( const char *errorMessage, ... )                        // (2) BSP
void BSPLib::VAbort( const char *errorMessage, va_list args )          // (3) Modern va_list
void bsp_vabort( const char *errorMessage, va_list args )              // (4) BSP va_list
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

If [`BSPLib::Init()`](../logic/init.md) is called after an abort, it will determine whether all threads have actually 
aborted. If not, it will notify the waiting threads about the abort. If even this fails, the entire program will terminate.
  

1. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.
3. Classic BSP function that takes a `va_list` directly.
4. Legacy BSP function that takes a `va_list` directly.

!!! note "Warnings"
    The termination behaviour is different in BSPLib differs from that in other BSP libraries.
  
!!! danger "Deprication"
    This function has been depricated and should be avoided.

#Parameters

* `format` Describes the message to format.
* `...`    Variable arguments providing message formatting.
* `args`   The formatting arguments.

#Post-Conditions
* All threads other than main thread will exit on a BSP call, unless the BSP process is restarted.
* If main thread, a BSPAbort exception will be thrown.
  
#Examples

###(1) Classic

```cpp
void main( int32_t, const char ** )
{
	bool success = BSPLib::Excute( []
	{
        if ( BSPLib::Classic::ProcId() == 0 )
        {
            // Prints "Quit non gracefully"
            BSPLib::Classic::Abort( "Quit non gracefully" );
            
            // Will never be reached
            while( true );
        }
        
        BSPLib::Classic::Sync();
        
	}, BSPLib::Classic::NProcs() );
    
    assert( !success );
}
```

###(2) BSP

```cpp
void main( int32_t, const char ** )
{
	bool success = BSPLib::Excute( []
	{
        if ( bsp_pid() == 0 )
        {
            // Prints "Quit non gracefully"
            bsp_abort( "Quit non gracefully" );
            
            // Will never be reached
            while( true );
        }
        
        bsp_sync();
        
	}, bsp_nprocs() );
    
    assert( !success );
}
```
