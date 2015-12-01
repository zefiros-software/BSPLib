#Interfaces

```cpp
void BSPLib::Classic::Init( std::function< void() > spmd, int32_t argc, char **argv) // (1) Classic
void bsp_init()                                                                      // (2) BSP
```

Initialises the BSP computation process. Please note that the main thread should also call the entry function.

#Parameters

* `spmd` The entry function to execute.
* `argc` *Currently not used, should be `argc` from `main` function.*
* `argv` *Currently not used, should be `argv` from `main` function.*

#Post-Conditions

 * Tag size is 0.
 * Entry point is now `entry`, thus when we call `Begin()` all threads except for the
   main thread will execute this function.
 * For the main thread, `ProcId() == 0`, other threads get their identifier assigned
   in the `Begin()` function.
   
#Deprication
This function has been depricated in favour of [`BSPLib::Execute()`](execute.md).
 
#Examples

###(1) Classic

This is the interface one should prefer to use, over the old BSP interface.

**SPMD Exectution**

In SPMD mode, BSPLib regocnises no [`BSPLib::Classic::Init`](init.md) has been called,
and thus automatically recalls the `main()` function for the other threads.

```cpp
void main(int32_t, const char **)
{
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


void main(int32_t argc, const char **argv)
{
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

###(2) BSP

This interface is included for backwards compatibility with other BSP libraries.

```cpp
void Spmd()
{  
    bsp_begin( bsp_nprocs() );
    
    std::cout << "Hello BSP Worldwide from process " << bsp_pid() 
              << " of " << bsp_nprocs() << std::endl;
              
    bsp_end();
}


void main(int32_t argc, const char **argv)
{
    bsp_init( Spmd, argc, argv );
    // Main thread needs to call it also
    Spmd();
}
```