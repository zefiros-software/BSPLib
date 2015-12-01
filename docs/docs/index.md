# BSPLib
[BSPLib](wwww.bsplib.eu) is a fast, and easy to use C++ implementation of the Bulk Synchronous Parallel (BSP) threading model.

#BSP
The BSP specification can be found [here](bsp.pdf). And is maintained by [bsp-worldwide.org](http://bsp-worldwide.org).

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
For detailed usage, please visit [BSPLib](www.bsplib.eu).

### Flexibility

#### Don't like the classic interface?
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

### BSPLib vs Oxford BSP Toolkit
* BSPLib is headeronly, whilst Oxford BSP Toolkit requires linking.

### BSPLib vs BSPonMPI
* BSPLib is headeronly, whilst BSPonMPI requires linking.
* BSPLib uses a permissive license (MIT), whereas BSPonMPI uses (LGPL).

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