# Truss
![](https://github.com/cyanray/truss/workflows/Build/badge.svg)
![](https://img.shields.io/github/license/cyanray/truss.svg)

A toy FEM solver written in C++20.


## Features
- [x] Ability to solve 3D problems.
- [x] Multiple elements:
  - [x] Truss
  - [x] Beam
  - [x] Constant Strain Triangle
  - [ ] More...
- [x] Multiple materials:
  - [x] Linear elastic
  - [ ] More...


## Project Structure
```
Truss
├──LibTrussDocument: Parser for parsing truss documents.
├──LibTrussSolver: The implementation of the solver.
└──TrussCLI: An executable program for loading truss document files and solving for the result.
```


## Dependencies
1. [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): Liner algebra library. (vcpkg)
2. [magic_enum](https://github.com/Neargye/magic_enum): Useful library for enum. (vcpkg) 
3. [Catch2](https://github.com/catchorg/Catch2): For unit testing. (vcpkg)
4. [backward-cpp](https://github.com/bombela/backward-cpp): For printing stack trace. (CMake FetchContent)

## How to use it?
In progress...

### Download and Compilation
...

### Examples
Check out the files in the TrussCLI/example_truss directory.


## License
MPL-2.0 license.
