#Interfaces

```cpp
bool Execute( std::function< void() > func, uint32_t nProc ) 						    // (1) Simple
bool Execute( std::function< void() > func, uint32_t nProc, int32_t argc, char **argv ) // (2) Arguments
```
This function executes the given function according to the BSP model, with the
given amount of processors. There is no need to seperate main thread and instanced threads from the library, 
and also [`BSPLib::Classic::Begin()`](begin.md) and [`BSPLib::Classic::End()`](end.md) are not needed in 
this function body. Everything is taken care off, and if the computations are abored prematurely the
return value will be false in stead of true.
 
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

#Return Value
True when the BSP program executed successfully, othwerise false.

#Post-Conditions

 * The BSP program has been executed.
  
#Examples

###(1) Simple

This is the most basic way of calling this function.

```cpp
void main( int32_t, const char ** )
{
	BSPLib::Excute( []
	{
		std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
				  << " of " << BSPLib::NProcs() << std::endl;
	}, BSPLib::NProcs() );
}
```

###(2) Arguments

When we need to pass the `main()` arguments to the BSP program.

```cpp
void main( int32_t argc, const char **argv )
{
	BSPLib::Excute( []
	{
		std::cout << "Hello BSP Worldwide from process " << BSPLib::Classic::ProcId() 
				  << " of " << BSPLib::NProcs() << std::endl;
	}, BSPLib::NProcs(), argc, argv );
}
```