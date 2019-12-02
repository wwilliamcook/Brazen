# Brazen

Brazen is an N-dimensional physics engine designed for modularity and extensibility. It provides a foundation upon which to build a variety of physics-based applications, and comes with modules for virtual camera output, virtual sensor output, virtual actuator input, and rudimentary audio output. It offers protocols for multithreaded asynchronous physics updates for real-time applications as well as a call-for-update mode for high temporal-resolution applications.

**NOTE: This project is still in its infancy and has no full releases yet. For a demonstration of its future capabilities, see the [old demos](docs/old-physics-engine) from its past life.**

## Getting Started

For a quick demonstration of Brazen's capabilities and how to use it within a program, see the applications in [`examples`](/examples/).

### Prerequisites

Brazen's core physics engine has no additional dependencies and can be used as-is. Additional dependencies are required for some of the included optional modules:

**Local Video Monitoring Module:**
* [SDL 2](https://www.libsdl.org)

**Audio Output Module:**
* [PortAudio](https://www.portaudio.com)

### Installing

As Brazen is a header-only C++ library, all that is required for installation is inclusion into your build directory paths.

Here is an example of building [`examples/bouncy_cube.cpp`](examples/bouncy_cube.cpp) using `g++`:

```
g++ -Iinclude examples/bouncy_cube.cpp
```

## Contributing

Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests to us.

## References

* [Directory Structure for a C++ Project](https://mariuszbartosik.com/directory-structure-for-a-c-project/)
* [Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/)

## Authors

* **Weston Cook** - *Initial work* - [wwilliamcook](https://github.com/wwilliamcook)

See also the list of [contributors](https://github.com/wwilliamcook/Brazen/contributors) who participated in this project.

## License

This project is licensed under the Mozilla Public License 2.0 - see the [`LICENSE`](LICENSE) file for details.
