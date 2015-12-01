#Interfaces

```cpp
void BSPLib::Classic::Init( std::function< void() > spmd, int32_t argc, char **argv)
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

**SPMD Exectution**

In SPMD mode, BSPLib regocnises no [`BSPLib::Classic::Init`](init.md) has been called,
and thus automatically recalls the `main()` function for the other threads.

```cpp
void main(int32_t, const char **)
{
    BSPLib::Classic::Begin(BSPLib::NProcs());
    
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
    BSPLib::Classic::Begin(BSPLib::NProcs());
    
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