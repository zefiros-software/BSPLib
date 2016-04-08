#Interfaces

```cpp
uint32_t BSPLib::NProcs()           // (1) Modern
uint32_t BSPLib::Classic::NProcs()  // (2) Classic
uint32_t bsp_nprocs()               // (3) BSP
```

Gets the the amount of processors used by the BSP library. When the BSP library is not
initialised this returns the amount of processors available in hardware. After initalisation
it returns the maximum amount of processors available as initialised. 

1. This is interface one should choose to use, and is semantically the same as the other interfaces.
2. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
3. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.

#Return Value

The amount of processors available.

#Pre-Conditions

 *     If Begin has been called:
    * Returns the number of started threads.
    
    Else:

    * Returns the number of physical threads available.
    
#Examples

###(1) Modern

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = BSPLib::NProcs();
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  BSPLib::NProcs() << std::endl;
    }, nProcs );
    
    // prints: "The amount of processors we started with equals 3", 3 times
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  BSPLib::NProcs() << std::endl;
    }, 3 );
}
```

###(2) Classic

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = BSPLib::Classic::NProcs();
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  BSPLib::Classic::NProcs() << std::endl;
    }, nProcs );
    
    // prints: "The amount of processors we started with equals 3", 3 times
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  BSPLib::Classic::NProcs() << std::endl;
    }, 3 );
}
```

###(3) BSP

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = bsp_nprocs();
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  bsp_nprocs() << std::endl;
    }, nProcs );
    
    // prints: "The amount of processors we started with equals 3", 3 times
    BSPLib::Execute( []
    {
        std::cout << "The amount of processors we started with equals " <<  bsp_nprocs() << std::endl;
    }, 3 );
}
```