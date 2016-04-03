# PlotLib
PlotLib is an easy to use, C++ plotting library that produces scientific publication quality ready images.

# Requirements
PlotLib makes use of [MatplotLib](http://matplotlib.org/) and [Seaborn](http://stanford.edu/~mwaskom/software/seaborn/) to produce its images.
PlotLib assumes that there is a full SciPy stack installed in the path. 

## Easy Installation

For Windows:
```
setup.bat
```

For Linux:
```
./setup.sh
```

## Manual Installation
The easiest installation is running [Anaconda](https://www.continuum.io/downloads).
Please note that Anaconda misses the Seaborn package which we install using:

```
pip install seaborn
```

## Features
* Reliable: We have an extensive testing suite, making sure we do not break forward and backward compatiblity.
* Easy to use: The library is completely header only.  
* Cross platform: Designed to only use c++2003, with no external dependencies.
* Warning free, compiled on the highest warning level.

## Installation
The whole library is header only, so for usage in your own code you should clone the repository. And add `serialisation/include/` as include path.
After this you can use the include `#include "serialisation/serialisation.h"` in your code to start using the library.

## Usage
The library interface only uses 4 functions, each with a different purpose:

 - Message::Store([reference], [index]); Stores primitives and objects.
 - Message::StoreParent([reference], [index]); Stores the variables from the given inherrited class.
 - Message::StoreContainer([reference], [index]); Stores container formats.
 - MessageHelper::Store([message], [reference]); Begins a store or load sequence.

To ensure backward and forward compatibility, each Store should be accompanied with a variable index. So when a variable
is removed, or added, this induces no problems. Note that each index in the classes should be unique.

#### Example
Say we start with:

```C++
class Foo
{
public:
	void OnStore( Message &message )
	{
		message.Store(mVar1, 0);
		message.Store(mVar2, 1);
		message.Store(mVar3, 2);
	}
	
private:
	uint32_t mVar1, mVar2;
	double mVar3;
};
```

#### Revision
However when we decide `mVar2` is not needed, and we need an extra `double` we can add it as such:

```C++
class Foo
{
public:

	void OnStore( Message &message )
	{
		message.Store(mVar1, 0);
		//message.Store(mVar2, 1);
		message.Store(mVar3, 2);
		message.Store(mVar4, 3);
	}
	
private:
	uint32_t mVar1;
	double mVar3, mVar4;
};
```

A good practice would be to uncomment deleted Stores, so the index does not get reused on accident!

### Primitives
The following types are considered primitives:

 - (u)int8_t
 - (u)int16_t
 - (u)int32_t
 - (u)int64_t
 - float
 - double
 - std::string

#### Example

```C++	
class Foo
{
public:

	void OnStore( Message &message )
	{
		message.Store(mVar1, 0);
		message.Store(mVar2, 1);
		message.Store(mVar3, 2);
	}
	
private:
	uint32_t mVar1, mVar2;
	double mVar3;
};
```

### Objects
There are serveral ways to store objects:

 - Using templates
 - Using interfaces
 - Using template specialisation

The following are all equivalent:

```C++
class Vec3Impl1
{
public:
	
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}


class Vec3Impl2
	: public ISerialisable
{
public:
	
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}


struct Vec3Impl3
{
	double mX, mY, mZ;
}

template<>
void MessageHelper::OnStore( Vec3Impl3 &serialisable, Message &message )
{
	message.Store( mX, 0 );
	message.Store( mY, 1 );
	message.Store( mZ, 2 );
}
```

#### Example
Storing an object from another object:

```C++		
class Vec3
{
public:
	
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}

class Foo
{
public:

	void OnStore( Message &message )
	{
		message.Store(mVar1, 0);
		message.Store(mVar2, 1);
	}
	
private:
	Vec3 mVar1, mVar2;
};
```

### Containers
Containers use the `StoreContainer` interface, and can store both primitives and objects. Currently the following containers are supported:

 - std::vector<>
 - std::array<> (c++11 only)
 - c-arrays
 - Any std-iterator compatible container format

Currently not yet supported:

 - Associative containers; (st::set<>, std::map<>)
 - Unordered associative containers


```C++
class Vec3
{
public:
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}

class Foo
{
public:
	void OnStore( Message &message )
	{
		message.StoreContainer(mVar1, 0);
		message.StoreContainer(mVar2, 1);
	}
	
private:
	std::vector< Vec3 > mVar1, mVar2;
};
```

### Inherritance
As you may have noticed we haven't touched the topic of inherritance yet. To be able to effectively use
inherritance, without having to spend your precious variable indices, we can use the `StoreParent` interface. The parents
use their own index system.

#### Example

```C++
class Vec3
{
public:
	
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}

class Vec4
	: public Vec3
{
public:
	
	void OnStore( Message &message )
	{
		message.StoreParent<Vec3>(*this, 0);
		message.Store(mW, 0);
	}
	
private:
	
	double mW;
}

class Foo
{
public:

	void OnStore( Message &message )
	{
		message.StoreContainer(mVar1, 0);
		message.StoreContainer(mVar2, 1);
	}
	
private:
	std::vector< Vec4 > mVar1, mVar2;
};
```

### Messages
Our message both serialises, and deserialises so we need to contruct the message object correctly.

```C++	
class Vec3
{
public:
	
	void OnStore( Message &message )
	{
		message.Store(mX, 0);
		message.Store(mY, 1);
		message.Store(mZ, 2);
	}
	
private:
	
	double mX, mY, mZ;
}

void main()
{
	Vec3 vec;
	std::stringstream ss;
	
	Message serMessage( ss, Format::Binary, Mode::Serialise );
	
	MessageHelper::Store( serMessage, vec );
	
	Message deserMessage( ss, Format::Binary, Mode::Deserialise );
	MessageHelper::Store( deserMessage, vec );
}
```

### Flexibility

#### Don't like OnStore?
If you dislike the name or if it is not in your style, you can change it to your needs.

```C++
#define SERIALISATION_CUSTOM_INTERFACE on_store
```

After this we changed the usage of the library to:

```C++
#include "serialisation/serialisation.h"

class Foo
{
public:

	void on_store( Message &message )
	{
		message.Store(mVar1, 0);
	}
	
private:
	uint32_t mVar1;
};
```

#### SerLib Limits
* Each class can use up to 27 indices, so we can store 27 seperate variables max.
* Each class can have up to 3 parents, we can only use 3 indices for parents.

## Planned Features
* Extensive documentation, on both design and useage.
* Compression, based on array type and with delta encoding.
	* FastPFOR integer compression.
	* FFPFOR for floating point compression.
* Static container serialisation. This feature will disable backward and forward compatibility on static containers,
  but doing so will improve the size and serialisation speed. All types will be stored and grouped by their own types.
* Message migrations, migrating your old message format to your new message formats with nice abstraction.
* Big endian platform support. (Only the runtime is not yet compatible)
* Pointer Patching: Creating objects based off reflection, and make sure references are kept between sessions.
* Reflection based serialisation: Define the outlay of your classes using a reflection library, which then can be used
  for serialisation.
* XML and JSON serialisation, based on reflection. No need for adjustments, and also makes conversion between binary and text formats possible.
* Extensive custom error handling.


## Alternatives
* [Boost Serialisation](http://www.boost.org/doc/libs/1_59_0/libs/serialization/doc/index.html)
* [Protocol Buffers](https://github.com/google/protobuf)
* [Cap'n Proto](https://capnproto.org/)

## Why SerLib?
We wanted an easy to use, non invasive serialisation library. Unfortunately no library exists that did implement
our requirements.

### SerLib vs Boost Serialisation
* Boost is a huge dependency, SerLib is header only.
* Boost uses operator overloading, that makes it non bindable. SerLib is c-interface compatible.
* Boost takes over your classes to work; SerLib can use already existing classes.

### SerLib vs Protocol Buffers
* Protocol Buffers uses a precompiler. SerLib is dynamic and header only.
* Protocol Buffers has issues on non unix systems. SerLib is simple c++2003, and is cross-platform.

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
We are interested to find out what projects use SerLib. We would love it to include your projects here, 
just shoot us a mail. :)

* Zefiros Engine 

## License
This project is licensed under the MIT license.

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
