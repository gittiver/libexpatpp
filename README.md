# libexpatpp - a modern C++ xml parser toolkit using expat

[![Travis CI Build Status](https://travis-ci.org/gittiver/libexpatpp.svg?branch=main)](https://travis-ci.org/gittiver/libexpatpp)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/gittiver/libexpatpp?svg=true)](https://ci.appveyor.com/project/gittiver/libexpatpp)
[![Packaging status](https://repology.org/badge/tiny-repos/libexpatpp.svg)](https://repology.org/metapackage/libexpatpp/versions)
[![Documentation Status](https://readthedocs.org/projects/libexpatpp/badge/?version=latest)](https://libexpatpp.readthedocs.io/en/latest/?badge=latest) 

A lightweight and modern C/C++ library for building xml parsers.
Currently a work in progress.

## Features ##

* wraps and build **expat** as part of library
* runs on all major platforms: Windows, OSX, linux
* provides an easy to use delegate class to build xml parsers
* [WORK IN PROGRESS]utilizing lambda for easy adding of handler callbacks
* [WORK IN PROGRESS]provides implementation of parser with stack of parsestates
* [WORK IN PROGRESS]xsdgen for generating C++ classes and parser from
  xsd schemata

## Contents
- [Features](#features)
- [Building](#building)
- [Using libexpatpp in your project](#using-libexpatpp-in-your-project)
- [API documentation](#api-documentation)
- [Running tests](#running-tests)
- [Development and contributing](#development-and-contributing)
- [Publication](#publication)
- [Acknowledgements](#acknowledgements)


## Building

libexpatpp can be built with <a href="https://cmake.org">CMake</a>.
CMake is able to produce a build environment for most platforms.
For the most important IDE exists a <a href="https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html">generator option</a> which allows to build project files for these systems.

The simplest (and platform independent) way to build the project using cmake
is executing the following command:

```
cmake -B <builddir> && cmake --build <builddir>
```

First command in chain creates the build directory \<builddir\> and configures the project.
Second command does a build of the default target in \<builddir\>.

Cmake can also be called with predefined options,
here for configuring and building a Release build:
```
cmake -D CMAKE_BUILD_TYPE=Release -B <builddir> && cmake --build <builddir>
```
## Using expatpp library in your project

### The "Hello World" example

The example hello_world contains a complete sample project using libexpatcpp and makes use of Cmake's FetchContent() Command to include, build and link expatpp library(explained more detailed in \ref approach1 .

Our "hello_world" project has just one source file, `hello_world.cpp` file, and it looks like this:

```
/**
 * \file hello_world.cpp the hello_world example for expatpp library
 * 
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "expatpp.hpp"

using std::cout;
using std::endl;

/** a parser delegate which extracts the character data from an xml element*/
class element_delegate : public xmlpp::abstract_delegate {
public:
  std::string fullname;
  std::string characterData;

  void onStartElement( const XML_Char *fullname, const XML_Char **atts) override {
    this->fullname = fullname;
  }
 
  void onCharacterData(const char *pBuf, int len) override {
    characterData.append(pBuf,len);
  }
};

int main(int argc,char** argv) {
  // prepare xml input, 
  const char* xml = "<p>hello world!</p>";

  // use the delegate to extract character data from input string
  element_delegate d;

  // parse xml input using delegate d's callbacks
  xmlpp::parser::result res = xmlpp::parser::parseString(xml,d);

  switch(res) {

  case xmlpp::parser::result::OK:
    // and output the extracted data in case of successful parsing
    cout << "element " << d.fullName << endl
	 << " contains this text: "<< d.characterData << endl;
    return EXIT_SUCCESS;

  default:
    cout << "error " << static_cast<int>(res)
	 << " on parsing " << endl;
    return -static_cast<int>(res);
  }
}
```

Running it should output

```
element p
contains this text: Hello World
```

for the CMake based build we have included a simple CMakeLists.txt: 

\include hello_world/CMakeLists.txt

To get you started quickly, let's take a look at a few ways to get simple Hello World project working.

### Approach #1: Use Cmake's FetchContent() Command

CMake contains the FetchContent() command to use libraries where a project depends on.

With:
```
FetchContent_Declare(expatpp         
    GIT_REPOSITORY "https://github.com/gittiver/libexpatpp" 
    GIT_TAG "v0_0_1" )
FetchContent_MakeAvailable(expatpp)
```

we include the library into the build.

With: 
```
target_link_libraries(your_exe expatpp)
```

we link our executable against libexpatpp.
This approach is used in the helloworld example, you can use the CMakeFile.lst from there as starting point for your project.

### Approach #2: Install libexpatpp library on machine.

You can install libraries by running in libexpatpp directory: 

```
cmake --build <builddir> --target install
```

Now, all you have to do to compile your project is `c++ helloWorld.cpp -o helloWorld -llibexpatpp`.

If you get error message like `cannot open shared object file: No such file or directory`, make sure that your linker includes path where libexpatpp was installed.

### Approach #3: Use as git submodule and use cmake in your project via CMake.

If you are using CMake for compilation, we suggest adding libexpatpp as a git submodule with the command
```
git submodule add https://github.com/gittiver/libexpatpp vendor/libexpatpp`
```

Afterwards, modify your top level CMakeLists.txt file accordingly:

```
add_subdirectory(vendor/libexpatpp EXCLUDE_FROM_ALL)
target_link_libraries(your_exe expatpp)
```

The `add_subdirectory` command adds a folder to the build tree, meaning it will run CMakeLists.txt from the included folder as well. Flag `EXCLUDE_FROM_ALL` disables building (and instalment) of targets in the added folder which are not needed in your project. In the above example only the (static) library `libexpatpp` will be build, while `libexpatpp-aligner`, `hello_world` and the rest won't. In order to access the `libexpatpp` API, add `#include "expatpp.hpp"` in your source file (CMake will automatically update your include path).

## API documentation

For complete documentation of libexpatpp library API, visit [http://gittiver.github.io/libexpatpp](http://gittiver.github.io/libexpatpp) (should be updated to the latest release).

To generate the latest API documentation yourself from the source, you need to have [doxygen](www.doxygen.org) installed.
Position yourself in the root directory and run `doxygen`, this will generate `docs/` directory. Then open `docs/html/index.html` file with you favorite browser.

## Running tests

## xsdgen - generate C++ code from xsd files

parses xml schemata and generates C++ code for types and elements
from schema
[TODO] generate also parser class for parsing schema

## Examples

### doxyml - parse doxygen xml output

the example parses the xml output of doxygen documentation system and
does a simple listing of its elements on standard output.


## Development and contributing
Feel free to send pull requests and raise issues.

When developing, you may want to use `-D CMAKE_BUILD_TYPE=Debug` flag when calling `cmake` in order to get debugging flags passed to compiler. This should also happen if you just run `cmake ..` with no flags, but I think I have noticed it does not always works as expected (probably has something to do with cmake cache). To check which flags is compiler using, run `make` with `VERBOSE=1`: `make VERBOSE=1`.


## Publication

## Acknowledgements

