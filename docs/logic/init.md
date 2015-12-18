#Interfaces

```cpp
void BSPLib::Classic::Init( std::function< void() > spmd, 
                            int32_t argc, char **argv)          // (1) Classic
void bsp_init( void(*spmd)(void), int32_t argc, char **argv)    // (2) BSP
```

Initialises the BSP computation process. Please note that the main thread should also call the entry function.

1. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
2. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.
   
!!! note "Warnings"
    * By default, [`BSPLib::Classic::Init()`](init.md) will print a warning to `stderr` in case an abort of the previous BSP program has been detected. This message can be suppressed by the symbol `BSP_SUPPRESS_ABORT_WARNING`.
    * When [`BSPLib::Classic::Init()`](init.md) failed to join all threads from the previous BSP program, an error will be printed to `stderr`, and the entire program will be terminated by `std::terminate()`. This behaviour cannot be disabled, as it will most certainly cause errors in the next BSP    program.
 
!!! danger "Deprication"
    This function has been depricated in favour of [`BSPLib::Execute()`](execute.md).

#Parameters

* `spmd` The entry function to execute.
* `argc` *Currently not used, should be `argc` from `main` function.*
* `argv` *Currently not used, should be `argv` from `main` function.*

#Pre-Conditions
 * If the previous BSP program was terminated using Abort or VAbort:
    * BSPLib will try to end the previous BSP program by notifying all threads that are stuck in a synchronisation about the previous Abort.
    * After 100 failed tries, the entire program will be terminated by `std::terminate()`.

#Post-Conditions

 * Tag size is 0.
 * Entry point is now `entry`, thus when we call [`BSPLib::Classic::Begin()`](begin.md) all threads except for the
   main thread will execute this function.
 * For the main thread, `BSPLib::ProcId() == 0`, other threads get their identifier assigned
   in the [`BSPLib::Classic::Begin()`](begin.md) function.

#Examples

###(1) Classic

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
    };

    // Set the entry point for the other threads
    BSPLib::Classic::Init( entry, argc, argv );
  
    // Main thread needs to call it also
    entry();
}
```

###(2) BSP

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