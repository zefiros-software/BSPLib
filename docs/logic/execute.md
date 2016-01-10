#Interfaces

```cpp
bool Execute( std::function< void() > func, uint32_t nProc )    // (1) Simple
bool Execute( std::function< void() > func, uint32_t nProc, 
              int32_t argc, char **argv )                       // (2) Arguments
```
This function executes the given function according to the BSP model, with the
given amount of processors. There is no need to seperate main thread and instanced threads from the library, 
and also [`BSPLib::Classic::Begin()`](begin.md) and [`BSPLib::Classic::End()`](end.md) are not needed in 
this function body. Everything is taken care off, and if the computations are abored prematurely,
the return value will be false instead of true.
 
This function is sematically equal to:
```cpp
std::function< void() > spmd = [func, nProc]
{
    BSPLib::Classic::Begin( nProc );

    func();

    BSPLib::Classic::End();
};

BSPLib::Classic::Init( spmd, argc, argv );

try
{
    spmd();
}
catch ( BspInternal::BspAbort & )
{
    return false;
}

return true;
```

A BSP program normally is paired by [`BSPLib::Classic::Begin()`](begin.md) 
and [`BSPLib::Classic::End()`](end.md) calls. When using the normal execution mode
[`BSPLib::Classic::Init()`](init.md) needs to be called beforehand, otherwise 
the computations will start in SPMD mode, and the behaviour will be undefined.
To solve this we introduce the [`BSPLib::Execute()`](execute.md) function makes the
BSP interface easier to use.

1. This is the most basic way of calling this function.
2. When we need to pass the `main()` arguments to the BSP program.

#Parameters

* `func`   The entry function to execute over all processors.
* `nProcs` The amount of processors to use in the executions.
* `argc`   *Currently not used, should be `argc` from `main` function.*
* `argv`   *Currently not used, should be `argv` from `main` function.*

#Return Value
True when the BSP program executed successfully, othwerise false.

#Post-Conditions

 * The BSP program has been executed.
  
#Examples

###(1) Simple

**Normal**

This is the normal usage of the function.

```cpp
void main( int32_t, const char ** )
{
    BSPLib::Execute( []
    {
        std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
                  << " of " << BSPLib::NProcs() << std::endl;
    }, BSPLib::NProcs() );
}
```

**Non Gracefull Exit**

If the computation could call abort, one can check for it on the return type.

```cpp
void main( int32_t, const char ** )
{
    bool success = BSPLib::Execute( []
    {
        std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
                  << " of " << BSPLib::NProcs() << std::endl;
                  
        if ( BSPLib::ProcId() == 0 )
        {
            BSPLib::Abort( "Quit non gracefully" );
        }
    }, BSPLib::NProcs() );
    
    assert( !success );
}
```

###(2) Arguments

```cpp
void main( int32_t argc, const char **argv )
{
    BSPLib::Execute( []
    {
        std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
                  << " of " << BSPLib::NProcs() << std::endl;
    }, BSPLib::NProcs(), argc, argv );
}
```