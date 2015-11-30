#Program Logic

[TOC]

###Execute

###Init

**Interface**

```c++
void BSPLib::Classic::Init( std::function< void() > spmd, int32_t argc, char **argv)
```

Initialises the BSP computation process. Please note that the main thread should also call the entry function.

**Parameters**

* `spmd` The entry function to execute.
* `argc` *Currently not used, should be `argc` from `main` function.*
* `argv` *Currently not used, should be `argv` from `main` function.*

**Post-Conditions**

 * Tag size is 0.
 * Entry point is now `entry`.
 * For the main thread, `ProcId() == 0`.

###Begin

**Interface**

```c++
void BSPLib::Classic::Begin( uint32_t p )
```

Begins the computations with the maximum given processors.
	 
**Parameters**

* `p` The maximum processors to use in computation.

**Pre-Conditions**

 * If Init has been called: 
	 * Will execute entry in parallel.
  	
	Else: 
 	  
	 * Will execute main in parallel. Also, a warning will be printed to stderr.
		 
 * If called from the main thread:
 
	* Resets and initialises the communication queues and buffers.
	* Calls the entry point for all other threads.
	* Starts the timer for the main thread.
   Else:
     Starts the timer for the current thread.

**Exceptions**

* `BspInternal::BspAbort`: 
  Thrown when an abort error condition occurs, if enabled with symbol `BSP_THROW`.

###End

**Interface**

```c++
void BSPLib::Classic::End()
```

Ends the BSP computation.

**Post-Conditions**

 * Begin has been called

**Pre-Conditions**

 * Ended is true
 * All threads have synced and ended.
 * The main thread releases all threads.