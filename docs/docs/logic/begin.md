#Interfaces

```cpp
void BSPLib::Classic::Begin( uint32_t p )
```

Begins the computations with the maximum given processors. The BSP model in implemented around the 
Single Program Multiple Data (SPMD) programming model. Which means that we divide our datastructures 
over all threads, and try to evenly divide the work per processor. BSPLib will handle the required 
communication, as implemented by the user. This means that the distribution of work
is the users responsibility, whilst the communication is that of the library.

Usually we create a BSP program by pairing (optionally) [`BSPLib::Classic::Init()`](init.md), [`BSPLib::Classic::Begin()`](begin.md) 
and [`BSPLib::Classic::End()`](end.md) calls. When no [`BSPLib::Classic::Init()`](init.md) call is made, the library resumes in 
SPMD mode, and calls the `main()` function in all threads. Otherwise the given entry point from the
[`BSPLib::Classic::Init()`](init.md) call will be used.

```spec
Like many other communications libraries, BSPlib adopts a Single Program Multiple Data (SPMD) programming model. The task of writing an SPMD program will typically involve
mapping a problem that manipulates a data structure of size `N` into `p` instances of a program that each manipulate an `N/p` sized block of the original domain. The role of
BSPlib is to provide the infrastructure required for the user to take care of the data distribution, and any implied communication necessary to manipulate parts of
the data structure that are on a remote process. An alternativerole for BSPlib is to provide an architecture independent target for higher-level libraries or programming tools that autom
atically distribute the problem domain among the processes.

Processes are created in a BSPlib program by the operations bsp_begin and bsp_end. They bracket a piece of code to be run in an SPMD manner on a number of processors. There
can only be one instance of a bsp_begin/bsp_end pair within a program. If bsp_begin and bsp_end are the first and last statements in a program, then the entire
BSPlib compu-tation is SPMD. An alternative mode is available where a single process starts execution and determines the number of parallel processes required for the calculation.
```
	 
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

#Deprication
This function has been depricated in favour of [`BSPLib::Execute()`](execute.md).

#Exceptions

* `BspInternal::BspAbort`: 
  Thrown when an abort error condition occurs, if enabled with symbol `BSP_THROW`.
  
#Examples

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

**Lambda Exectution**

A more advanced manner of execution is using lambda, giving our 
more freedom over small BSP programs. Since we now use `std::function< void() >`
as entry point, we are no longer bound by function pointers. [`BSPLib::Execute()`](execute.md)
is a nice abstraction over this programming method, and should be used in favour of calling the
program logic functions yourself.

```cpp
void main( int32_t argc, const char **argv )
{
	auto entry = []
	{
		BSPLib::Classic::Begin( BSPLib::NProcs() );
		
		std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
				      << " of " << BSPLib::NProcs() << std::endl;
				
		BSPLib::Classic::End();
	}
	
	// Set the entry point for the other threads
    BSPLib::Classic::Init( entry, argc, argv );
	
    // Main thread needs to call it also
    entry();
}
```