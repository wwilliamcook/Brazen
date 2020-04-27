# Brazen

Brazen is an N-dimensional physics engine designed for modularity and extensibility, and provides a foundation upon which to build a variety of physics-based applications. It offers a protocol for multithreaded asynchronous physics updates for real-time applications as well as a synchronous, call-for-update mode for high temporal resolution applications.

## Getting Started

Brazen's core physics engine is a header-only C++ library and can be used just by adding the `include` folder to the include path of a C++ project.

If you wish to view the examples, you will need to download the SDL2 source code ([here](https://www.libsdl.org/download-2.0.php)) and move it to `Brazen/deps/SDL2-2.X.X`, renaming it to `Brazen/deps/SDL2`.

You may then create a build project using CMake and compile using the compiler of your choice. See [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/) for instructions on how to install and use CMake.

**NOTE**: examples using SDL graphics, such as `spectate.cpp`, must have `circle.bmp` in the same folder as their executable in order to run.

## References

* [Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/)
* [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/)

## Authors

* **Weston Cook** - [wwilliamcook](https://github.com/wwilliamcook)

## License

This project is distributed under the Mozilla Public License 2.0 - see the [`LICENSE`](LICENSE) file for details.
