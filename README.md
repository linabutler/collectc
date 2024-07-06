# collectc

**collectc** is a library of collection types for C. Its goal is to provide understandable implementations of common sequence, map, and set data structures.

**collectc** is inspired by [Rust's](https://doc.rust-lang.org/std/collections/index.html) and [Swift's](https://developer.apple.com/documentation/swift/collections) wonderful collection libraries, and I hope that it can be a project for learning and teaching C, as well as a building block for modern C projects.

This library is very much a work in progress, and I'd love to hear if *collectc* is helpful for you! Please feel free to create issues, pull requests, and discussions on [GitHub](https://github.com/linabutler/collectc).

### Building

#### Dependencies

* [CMake](https://cmake.org/) 3.29 or higher.
* A C11-capable compiler, like [Clang](https://clang.llvm.org/) 3.2, [GCC](https://gcc.gnu.org/) 4.9, or [Visual Studio](https://www.visualstudio.com/vs/community/) 2019 version 16.8; or higher.
* A CMake-compatible build system, like Make, [Ninja](https://ninja-build.org/), or Visual Studio.
* Optionally, [Doxygen](https://www.doxygen.nl/) 1.11 or higher, for generating the documentation.

Once you've installed the dependencies, you can create a CMake build directory, and generate the input files for your build system:

```shell
$ mkdir build
$ cmake [-G Ninja | "Unix Makefiles" | ...] -S ./ -B build/
```

#### macOS and \*nix

To build **collectc** as a static library:

```shell
$ cmake --build build/ -t collectc [--config Debug | Release]
```

To build and run the tests:

```shell
$ cmake --build build/ -t test [--config Debug | Release]
```

To generate and view the documentation:

```shell
$ cmake --build build/ -t collectc-doc
$ open build/html/index.html
```

### License

**collectc** is open source, and distributed under the Mozilla Public License, version 2.0.
