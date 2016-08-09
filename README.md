# BSPLib
[BSPLib](https://wwww.bsplib.eu) is a fast, and easy to use C++ implementation of the Bulk Synchronous Parallel (BSP) threading model. This model is mainly used in the scientific computing field, but can also be applied more general in computer science. This library includes an extended version the standard BSP interface to perform better, and made it more easy to use.

# Status
OS          | Status
----------- | -------
Linux & OSX | [![Build Status](https://travis-ci.org/Zefiros-Software/BSPLib.svg?branch=master)](https://travis-ci.org/Zefiros-Software/BSPLib)
Windows     | [![Build status](https://ci.appveyor.com/api/projects/status/g2aduvs6k7u3640r?svg=true)](https://ci.appveyor.com/project/PaulVisscher/bsplib)

#Documentation
The complete documentation can be found [here](http://bsplib.eu/).

## Why BSPLib?
* Fast: All our code is hand optimised using profilers, to ensure maximum performance.
* Reliable: We have an extensive testing suite, making sure we do not break forward and backward compatiblity.
* Easy to use: The library is completely header only. Also next to our improved c++ interface, we shipped the
classic BSP C interface.
* Cross platform: Designed to only use C++11, with no external dependencies.
* Warning free, compiled on the highest warning level.

## Bugs
When a bug is found, please insert it in the issue tracker, so we can resolve it as quickly as we can.

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request