# Truss
A toy FEM solver written in C++20.


## Features
- [x] Ability to solve 3D problems.
- [x] Multiple elements:
  - [x] Truss
  - [x] Beam
  - [ ] More...
- [x] Multiple materials:
  - [x] Linear elastic
  - [ ] More...


## Project Structure
```
Truss
├──LibTrussDocument: Parser for parsing truss documents.
├──LibTrussSolver: The implementation of the solver.
└──TrussCLI: An executable program for loading truss documents and solving their problems.
```


## Dependencies
1. [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): Liner algebra library.
2. [magic_enum](https://github.com/Neargye/magic_enum): Useful library for enum.
3. [Catch2](https://github.com/catchorg/Catch2): For unit testing.


## How to use it?
In progress...

### Download and Compilation
...

### Examples
Check out the files in the TrussCLI/example_truss directory.


## License
MPL-2.0 license.