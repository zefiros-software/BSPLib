#Interfaces

```cpp
double BSPLib::Time()            // (1) Modern
double BSPLib::Classic::Time()  // (2) Classic
double bsp_time()                // (3) BSP
```
Gets the time in seconds since for this thread since starting calculations by
calling [`BSPLib::Classic::Begin()`](../logic/begin.md). This function internally
uses a high precion time, however a good resolution is not guaranteed and may vary
between platforms and compilers. The timings are thread specific, and
may vary, due to startup times.

1. This is interface one should choose to use, and is semantically the same as the other interfaces.
2. Classic BSP function, this is the interface one should prefer to use over the old BSP interface.
3. Legacy BSP function, this interface is included for backwards compatibility with other BSP libraries.

#Return Value

The the time in seconds for this processor since the beginning of computation.

#Examples

###(1) Modern

```cpp
void main( int32_t, const char ** )
{
    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The processes waited for `x` seconds", where `x` is greater
    // than 5 for each thread. 
    BSPLib::Execute( []
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
        std::cout << "The processes waited for " <<  BSPLib::Time() 
                  << " seconds" << std::endl;
    }, nProcs );
}
```

###(2) Classic

```cpp
void main( int32_t, const char ** )
{
    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The processes waited for `x` seconds", where `x` is greater
    // than 5 for each thread. 
    BSPLib::Execute( []
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
        std::cout << "The processes waited for " <<  BSPLib::Classic::Time() 
                  << " seconds" << std::endl;
    }, nProcs );
}
```

###(3) BSP

```cpp
void main( int32_t, const char ** )
{

    const uint32_t nProcs = BSPLib::NProcs();
  
    // prints: "The processes waited for `x` seconds", where `x` is greater
    // than 5 for each thread. 
    BSPLib::Execute( []
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
        std::cout << "The processes waited for " <<  bsp_sleep() 
                  << " seconds" << std::endl;
    }, nProcs );
}
```