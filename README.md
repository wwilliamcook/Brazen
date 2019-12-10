# Brazen

Brazen is an N-dimensional physics engine designed for modularity and extensibility, and provides a foundation upon which to build a variety of physics-based applications. It offers a protocol for multithreaded asynchronous physics updates for real-time applications as well as a synchronous, call-for-update mode for high temporal-resolution applications.

## Prerequisites

Brazen's core physics engine is in the form of a header-only C++ library and can be used as is. The following dependencies are optional:

* [SDL2](https://www.libsdl.org/) - required to view the simulated environment

See [Getting Started](#getting-started) below for dependency installation instructions.

## Getting Started

After downloading the repository, follow the steps below to install SDL in your respective environment.

### Linux

Go to SDL's [download page](https://www.libsdl.org/download-2.0.php) and download the source code. Create a new folder called `deps` in the root of the Brazen repository and extract the downloaded file, renaming the extracted folder from `SDL2-2.X.X` to `SDL2-make` and placing it in `deps`. Then navigate to `SDL2-make` and run the following commands in a terminal to build SDL:

```
$ ./configure
$ make all
```

You will also need to install the SDL development library package using `$ sudo apt install libsdl2-dev` for `APT` or `$ yum install SDL2-devel` for `YUM`. You may then compile the examples using `$ make examples` from the root directory of Brazen, then run an example such as `spectate.cpp` using `$ ./bin/example/spectate`.

**NOTE**: examples using SDL graphics, such as `spectate.cpp`, must have `circle.bmp` in the working directory of their executable in order to run.

### Windows (Visual Studio)

Go to SDL's [download page](https://www.libsdl.org/download-2.0.php) and download the Windows Visual C++ development library. Create a new folder called `deps` in the root of the Brazen repository and extract the downloaded file, renaming the extracted folder from `SDL2-2.X.X` to `SDL2-vs` and placing it in `deps`. You may then open `VisualC++\Brazen.sln` using Visual Studio to build examples and tests. Finally, go to `deps\SDL2-vs\lib\x86` and `deps\SDL2-vs\lib\x64` and copy `SDL2.dll` into the respective build folders of the 32-bit and 64-bit executables.

**NOTE**: examples using SDL graphics, such as `spectate.cpp`, must have `circle.bmp` and `SDL2.dll` in the same folder as their executable in order to run.

## References

* [Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/)

## Authors

* **Weston Cook** - [wwilliamcook](https://github.com/wwilliamcook)

## License

This project is distributed under the Mozilla Public License 2.0 - see the [`LICENSE`](LICENSE) file for details.
