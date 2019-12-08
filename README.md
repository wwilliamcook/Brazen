# Brazen

Brazen is an N-dimensional physics engine designed for modularity and extensibility, and provides a foundation upon which to build a variety of physics-based applications. It offers protocols for multithreaded asynchronous physics updates for real-time applications as well as a call-for-update mode for high temporal-resolution applications.

## Prerequisites

Brazen's core physics engine is in the form of a header-only C++ library and can be used as is. The following dependencies are optional:

* [Simple DirectMedia Layer 2.0](https://www.libsdl.org/) - required to view the simulated environment

## Getting Started

After downloading this repository, follow the steps below to install Simple DirectMedia Layer in your respective environment.

### Linux

Go to SDL's [download page](https://www.libsdl.org/download-2.0.php) and download the source code. Extract the downloaded file and place `SDL2-2.X.X` into Brazen's `deps` folder as `SDL2-make`. Then navigate to `SDL2-make` and run the following commands in a terminal to build SDL:

```
$ ./configure
$ make all
```

You may then compile the examples using `$ make examples` from the root directory of Brazen, then run an example such as `spectate.cpp` using `$ ./bin/example/spectate`

### Windows (Visual Studio)

Go to SDL's [download page](https://www.libsdl.org/download-2.0.php) and download the Windows Visual C++ development library. Extract the downloaded file and place `SDL2-2.X.X` into the `deps` folder as `SDL2-vs`. You may then open `VisualC++\Brazen.sln` using Visual Studio to build examples and tests.

## References

* [Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/)

## Authors

* **Weston Cook** - [wwilliamcook](https://github.com/wwilliamcook)

## License

This project is distributed under the Mozilla Public License 2.0 - see the [`LICENSE`](LICENSE) file for details.
