libexpatpp - a modern C++ xml parser toolkit using expat                        {#mainpage}
=====

[![Travis CI Build Status](https://travis-ci.org/gittiver/libexpatpp.svg?branch=main)](https://travis-ci.org/gittiver/libexpatpp)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/gittiver/libexpatpp?svg=true)](https://ci.appveyor.com/project/gittiver/libexpatpp)
[![Packaging status](https://repology.org/badge/tiny-repos/libexpatpp.svg)](https://repology.org/metapackage/libexpatpp/versions)
[![Documentation Status](https://readthedocs.org/projects/libexpatpp/badge/?version=latest)](https://libexpatpp.readthedocs.io/en/latest/?badge=latest) 

A lightweight and modern C/C++ library for building xml parsers

## Features
* wraps  **expat**.


## Contents
- [Features](#features)
- [Building](#building)
- [Using libexpatpp in your project](#using-libexpatpp-in-your-project)
- [Usage and examples](#usage-and-examples)
- [API documentation](#api-documentation)
- [Running tests](#running-tests)
- [Development and contributing](#development-and-contributing)
- [Publication](#publication)
- [Acknowledgements](#acknowledgements)


## Building

### CMake
libexpatpp can be built with CMake.

Execute following command to build libexpatpp using CMAKE:
```
cd build && cmake -D CMAKE_BUILD_TYPE=Release .. && make
```
This will create binaries in `bin/` directory and libraries (static and shared) in `lib/` directory.

```
./bin/runTests
```
to run tests.

Optionally, you can run
```
sudo make install
```
to install libexpatpp library on your machine.

## Using libexpatpp in your project
You can use libexpatpp in you project by either directly copying header and source files from [libexpatpp/](libexpatpp/), or by linking libexpatpp library (see [Building](#building) for instructions how to build libexpatpp libraries).
In any case, only thing that you have to do in your source files is to include `libexpatpp.h`.

To get you started quickly, let's take a look at a few ways to get simple Hello World project working.

Our Hello World project has just one source file, `helloWorld.cpp` file, and it looks like this:
```cpp
#include <cstdio>
#include "libexpatpp.h"

int main() {
}
```

Running it should output `???`.

### Approach #2: Copying libexpatpp header file and static library.
Instead of copying libexpatpp source files, you could copy static library (check [Building](#building) on how to create static library). We also need to copy libexpatpp header files. We get following project structure:
```
libexpatpp/  -> copied from libexpatpp
  include/
    libexpatpp.h
  libexpatpp.a
helloWorld.cpp -> your program
```

Now you can compile it with `c++ helloWorld.cpp -o helloWorld -I libexpatpp/include -L libexpatpp -llibexpatpp`.

### Approach #3: Install libexpatpp library on machine.
Alternatively, you could avoid copying any libexpatpp files and instead install libraries by running `sudo make install` (check [Building](#building) for exact instructions depending on approach you used for building). Now, all you have to do to compile your project is `c++ helloWorld.cpp -o helloWorld -llibexpatpp`.
If you get error message like `cannot open shared object file: No such file or directory`, make sure that your linker includes path where libexpatpp was installed.

### Approach #4: Use cmake in your project via CMake.
If you are using CMake for compilation, we suggest adding libexpatpp as a git submodule with the command `git submodule add https://github.com/martinsos/libexpatpp vendor/libexpatpp`. Afterwards, modify your top level CMakeLists.txt file accordingly:
```
add_subdirectory(vendor/libexpatpp EXCLUDE_FROM_ALL)
target_link_libraries(your_exe libexpatpp) # or target_link_libraries(your_exe libexpatpp)
```
The `add_subdirectory` command adds a folder to the build tree, meaning it will run CMakeLists.txt from the included folder as well. Flag `EXCLUDE_FROM_ALL` disables building (and instalment) of targets in the added folder which are not needed in your project. In the above example only the (static) library `libexpatpp` will be build, while `libexpatpp-aligner`, `hello_world` and the rest won't. In order to access the `libexpatpp` API, add `#include "libexpatpp.h"` in your source file (CMake will automatically update your include path).


For more example projects take a look at applications in [apps/](apps/).


## Usage and examples

## API documentation

For complete documentation of libexpatpp library API, visit [http://gittiver.github.io/libexpatpp](http://gittiver.github.io/libexpatpp) (should be updated to the latest release).

To generate the latest API documentation yourself from the source, you need to have [doxygen](www.doxygen.org) installed.
Position yourself in the root directory and run `doxygen`, this will generate `docs/` directory. Then open `docs/html/index.html` file with you favorite browser.

## Running tests

## Examples

### doxyml - parse dxygen xml output

## Development and contributing
Feel free to send pull requests and raise issues.

When developing, you may want to use `-D CMAKE_BUILD_TYPE=Debug` flag when calling `cmake` in order to get debugging flags passed to compiler. This should also happen if you just run `cmake ..` with no flags, but I think I have noticed it does not always works as expected (probably has something to do with cmake cache). To check which flags is compiler using, run `make` with `VERBOSE=1`: `make VERBOSE=1`.


## Publication

## Acknowledgements


