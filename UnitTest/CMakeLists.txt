cmake_minimum_required(VERSION 3.20)
project(Truss-UnitTest)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(LibTrussDocument)
add_subdirectory(LibTrussSolver)