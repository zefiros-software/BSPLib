#Interfaces

```cpp
void BSPLib::Classic::End()
```

Ends the BSP computation. All threads must call this function to gracefully exit the BSP program. A BSP program is paired by [`BSPLib::Classic::Begin()`](begin.md) 
and [`BSPLib::Classic::End()`](end.md) calls. Wherein the [`BSPLib::Classic::End()`](end.md) should be the last call in the computation. Calling this function
will ensure that the library is reset for reuse, and thuse we can restart calculations or start another SPMD program.

#Pre-Conditions

 * Begin has been called

#Post-Conditions

 * Ended is true
 * All threads have synced and ended.
 * The main thread releases all threads.
 
#Deprication
This function has been depricated in favour of [`BSPLib::Execute()`](execute.md).
 
#Examples

```cpp
void main( int32_t, const char ** )
{
	BSPLib::Classic::Begin( BSPLib::NProcs() );
	
	std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
			  << " of " << BSPLib::NProcs() << std::endl;
	
	// Stop all the threads, reset the library
	// and resume execution on the main thread.
	BSPLib::Classic::End();
}
```