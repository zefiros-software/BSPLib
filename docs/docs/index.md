# BSPLib
[BSPLib](https://wwww.bsplib.eu) is a fast, and easy to use C++ implementation of the Bulk Synchronous Parallel (BSP) threading model.

#BSP
The BSP specification can be found [here](bsp.pdf) and is maintained by [bsp-worldwide.org](http://bsp-worldwide.org). Parts of the
documentation and code are based off this specification, to ensure backward compatibility between BSP libraries.

#Code
The code can be found [here](https://github.com/Zefiros-Software/BSPLib).

## Features
* Fast: All our code is hand optimised using profilers, to ensure maximum performance.
* Reliable: We have an extensive testing suite, making sure we do not break forward and backward compatiblity.
* Easy to use: The library is completely header only. Also next to our improved c++ interface, we shipped the
classic BSP C interface.
* Cross platform: Designed to only use C++11, with no external dependecies.
* Warning free, compiled on the highest warning level.

## Installation
The whole library is header only, so for usage in your own code you should clone the repository. And add `bsp/include/` as include path.
After this you can use the include `#include "bsp/bsp.h"` in your code to start using the library.

## Usage
For detailed usage, please visit [BSPLib](https://www.bsplib.eu).

**Example**

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

### Flexibility

#### Don't like the legacy interface?
No worries! You can easily disable the macros by defining:
```cpp
#define BSP_DISABLE_LEGACY
```

#### Speeding up debugging
When debug builds are too slow, you can remove the extra safety check by defining:
```cpp
#define BSP_SKIP_CHECKS
```

#### BSPLib Limits
* For small programs, you may experience a lot of overhead in starting the threads.
* Starting more threads than available physical cores, may reduce perfomance.
* No support for more nodes by TCP/UDP connections.

## Planned Features
* MultiBSP interface addition.
* Utility functions, such as broadcasting and various distributions.
* Subset synchronisation on BSPLib::Sync with both predicates and processors lists.
  eg. BSPLib::Sync( [] { return BSPLib::ProcId() % 2 == 0; } ) and BSPLib::Sync( {1, 3, 4} )
* BenchLib version of BSP bench, so we can circumvent compiler optmisations and differences.

## BSPedupack
As an example usage of the BSP programming model we included the BSPedupack written by [Rob H. Bisseling](http://www.staff.science.uu.nl/~bisse101/)
with his permission. It includes some tools to benchmark and test the library on different machines, to quote
from his own [site](http://www.staff.science.uu.nl/~bisse101/Software/software):

>*BSPedupack is a library of numerical algorithms written in C according to the BSP model. 
It uses the BSPlib standard library. The 'edu' in the name stands for educational. 
The programs are intended for teaching: they give examples of portable parallel 
numerical programs. The trade-off between clarity/brevity on the one hand, and efficiency 
on the other hand is in favour of clarity/brevity. There is also an MPI version which 
teaches how to program in bulk synchronous parallel style using the collective communications
from MPI-1 and the one-sided communications from MPI-2. You guessed it, this package is called 
MPIedupack. Both packages accompany the book: [Parallel Scientific Computation: 
A Structured Approach using BSP and MPI](http://ukcatalogue.oup.com/product/9780198529392.do), 
by [Rob H. Bisseling](http://www.staff.science.uu.nl/~bisse101/), Oxford University Press, 
March 2004. ISBN 978-0-19-852939-2.*

## Alternatives
* [MulticoreBSP](http://www.multicorebsp.com)
* [Oxford BSP Toolkit](http://www.bsp-worldwide.org/implmnts/oxtool/)
* [BSPonMPI](http://bsponmpi.sourceforge.net/)

## Why BSPLib?
We wanted an easy to use, high performing BSP model implementation, but of course there
were some alternatives.

### BSPLib vs MulticoreBSP
* BSPLib is headeronly, whilst MulticoreBSP requires linking.
* BSPLib is crossplatform, but MulticoreBSP requires POSIX compatibility.
* MulticoreBSP can be compiled with a C compiler.
* MulticoreBSP programs will compile faster.
* BSPLib can disable sanity checks to boost performance.
* BSPLib will only synchronise the used functionality, so we have faster synchronisation.

### BSPLib vs Oxford BSP Toolkit
* BSPLib is headeronly, whilst Oxford BSP Toolkit requires linking.
* Oxford BSP Toolkit programs will compile faster.

### BSPLib vs BSPonMPI
* BSPLib is headeronly, whilst BSPonMPI requires linking.
* BSPLib uses a permissive license (MIT), whereas BSPonMPI uses (LGPL).
* BSPonMPI is based on MPI, so it uses another abstraction layer, whereas BSPLIb uses a low level
  threading interface.
* BSPonMPI programs will compile faster.

## Bugs
When a bug is found please insert it in the issue tracker, so we can resolve it as quickly as we can.

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## Authors
* Mick van Duijn <mick.v.duijn@zefiros.eu>
* Paul Visscher <paul.pev@zefiros.eu>
* Koen Visscher <koen.visscher@zefiros.eu>

## Used By
We are interested to find out what projects use BSPLib. We would love it to include your projects here, 
just shoot us a mail. :)

## License
This project is licensed under the MIT license.

```
Copyright (c) 2015 Mick van Duijn, Koen Visscher and Paul Visscher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```