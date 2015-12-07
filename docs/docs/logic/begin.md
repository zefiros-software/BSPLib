#Interfaces

```cpp
void BSPLib::Classic::Begin( uint32_t p ) // (1) Classic
void bsp_begin( uint32_t p )              // (2) BSP
```

Begins the computations with the maximum given processors. The BSP model is implemented around the 
Single Program Multiple Data (SPMD) programming model. This means that we divide our datastructures 
over all threads, and try to evenly divide the work per processor. BSPLib will handle the required 
communication, as implemented by the user. This means that the distribution of work
is the users responsibility, whilst the communication is that of the library.

Usually we create a BSP program by pairing (optionally) [`BSPLib::Classic::Init()`](init.md), [`BSPLib::Classic::Begin()`](begin.md) 
and [`BSPLib::Classic::End()`](end.md) calls. When no [`BSPLib::Classic::Init()`](init.md) call is made, the library resumes in 
SPMD mode, and calls the `main()` function in all threads. Otherwise the given entry point from the
[`BSPLib::Classic::Init()`](init.md) call will be used.

!!! danger "Deprication"
    This function has been depricated in favour of [`BSPLib::Execute()`](execute.md).
	 
#Parameters

* `p` The maximum processors to use in computation. Assigning more processors than 
      physically available could slow down your program.

#Pre-Conditions

 * If Init has been called: 
	 * Will execute entry in parallel.
  	
	Else: 
 	  
	 * Will execute main in parallel. Also, a warning will be printed to `stderr`.
		 
 * If called from the main thread:
 
	* Resets and initialises the communication queues and buffers.
	* Calls the entry point for all other threads.
	* Starts the timer for the main thread.
   Else:
     Starts the timer for the current thread.

#Exceptions

* `BspInternal::BspAbort`: 
  Thrown when an abort error condition occurs, if enabled with symbol `BSP_THROW`.
  
#Examples

###(1) Classic

This is the interface one should prefer to use, over the old BSP interface.

**SPMD Exectution**

In SPMD mode, BSPLib regocnises no [`BSPLib::Classic::Init`](init.md) has been called,
and thus automatically recalls the `main()` function for the other threads.

```cpp
void main( int32_t, const char ** )
{
	  // No Init call, so other threads will call
	  // the main fuction
    BSPLib::Classic::Begin( BSPLib::NProcs() );
    
    std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
              << " of " << BSPLib::NProcs() << std::endl;
    
    BSPLib::Classic::End();
}
```

Of course namespaces may be ommited, so the interface gets even simpler.

```cpp
void main( int32_t, const char ** )
{
    using namespace BSPLib;
    
	  // No Init call, so other threads will call
	  // the main fuction
    Classic::Begin( NProcs() );
    
    std::cout << "Hello BSP Worldwide from process " << ProcId() 
              << " of " << NProcs() << std::endl;
    
    Classic::End();
}
```

**Normal Exectution**

In the normal execution mode we need to initialise the BSPLibrary with a [`BSPLib::Classic::Init`](init.md) call.
This ensures we assign the correct entry point to the threads, which will be called on the
[`BSPLib::Classic::Begin()`](begin.md) call.

```cpp
void Spmd()
{  
    BSPLib::Classic::Begin( BSPLib::NProcs() );
    
    std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
              << " of " << BSPLib::NProcs() << std::endl;
              
    BSPLib::Classic::End();
}


void main( int32_t argc, const char **argv )
{
	  // Set the entry point for the other threads
    BSPLib::Classic::Init( Spmd, argc, argv );
	
    // Main thread needs to call it also
    Spmd();
}
```

###(2) BSP

This interface is included for backwards compatibility with other BSP libraries.

```cpp
void main( int32_t, const char ** )
{
  // No Init call, so other threads will call
  // the main fuction
  bsp_begin( bsp_nprocs() );
  
  std::cout << "Hello BSP Worldwide from process " << bsp_pid() 
            << " of " << bsp_nprocs() << std::endl;
  
  bsp_end();
}
```