#Interfaces

```cpp
uint32_t BSPLib::NProcs()			// (1) Modern
uint32_t BSPLib::Classic::NProcs()  // (2) Classic
uint32_t bsp_nprocs()				// (3) BSP
```

Gets the the amount of processors used by the BSP library. When the BSP library is not
initialised this returns the amount of processors available in hardware. After initalisation
it returns the maximum amount of processors available as initialised. 

#Return Value

The amount of processors available.

#Pre-Conditions

 * 	If Begin has been called:
	* Returns the number of started threads.
	
	Else:

	* Returns the number of physical threads available.
	
#Examples

###(1) Modern

This is interface one should choose to use.

```cpp
void main( int32_t, const char ** )
{
	// The total amount of physical processors available
	const uint32_t nProcs = BSPLib::NProcs();
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  BSPLib::NProcs() << std::endl;
	}, nProcs );
	
	// prints: "The amount of processors we started with equals 3", 3 times
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  BSPLib::NProcs() << std::endl;
	}, 3 );
}
```

###(2) Classic

This is the implementation of the classic interface, in a modern style.

```cpp
void main( int32_t, const char ** )
{
	// The total amount of physical processors available
	const uint32_t nProcs = BSPLib::Classic::NProcs();
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  BSPLib::Classic::NProcs() << std::endl;
	}, nProcs );
	
	// prints: "The amount of processors we started with equals 3", 3 times
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  BSPLib::Classic::NProcs() << std::endl;
	}, 3 );
}
```

###(3) BSP

This interface is included for backwards compatibility with other BSP libraries.

```cpp
void main( int32_t, const char ** )
{
	// The total amount of physical processors available
	const uint32_t nProcs = bsp_nprocs();
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  bsp_nprocs() << std::endl;
	}, nProcs );
	
	// prints: "The amount of processors we started with equals 3", 3 times
	BSPLib::Excute( []
	{
		std::cout << "The amount of processors we started with equals " <<  bsp_nprocs() << std::endl;
	}, 3 );
}
```