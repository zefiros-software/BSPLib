#Interfaces

```cpp
uint32_t BSPLib::ProcId()            // (1) Modern
uint32_t BSPLib::Classic::ProcId()  // (2) Classic
uint32_t bsp_pid()                    // (3) BSP
```

When [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called, all processors other than the main thread
will get a unique identifier assigned in the range [`[1, [BSPLib::NProcs() - 1]`](nprocs.md). The main 
thread holds identifier `0`.

1. This is interface one should choose to use, and is semantically the same as the other interfaces.
2. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
3. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.

#Return Value

The current processor identifier.

#Pre-Conditions

* [`BSPLib::Classic::Begin()`](../logic/begin.md) has been called, otherwise
  this will return `0xdeadbeef`.
  
  
#Examples

###(1) Modern

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The current processor identifier is `x`", with x between 0 and `nProcs` - 1.
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  BSPLib::ProcId() << std::endl;
    }, nProcs );
    
    // prints: "The current processor identifier is 0"
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  BSPLib::ProcId() << std::endl;
  }, 1 );
}
```

###(2) Classic

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The current processor identifier is `x`", with x between 0 and `nProcs` - 1.
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  BSPLib::Classic::ProcId() << std::endl;
    }, nProcs );
    
    // prints: "The current processor identifier is 0"
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  BSPLib::Classic::ProcId() << std::endl;
  }, 1 );
}
```

###(3) BSP

```cpp
void main( int32_t, const char ** )
{
    // The total amount of physical processors available
    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The current processor identifier is `x`", with x between 0 and `nProcs` - 1.
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  bsp_pid() << std::endl;
    }, nProcs );
    
    // prints: "The current processor identifier is 0"
    BSPLib::Excute( []
    {
        std::cout << "The current processor identifier is " <<  bsp_pid() << std::endl;
  }, 1 );
}
```