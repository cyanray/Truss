cmake_minimum_required(VERSION 3.20)

find_package(Catch2 3 REQUIRED)

add_executable(LibTrussSolverTests test.cpp)
target_link_libraries(LibTrussSolverTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(LibTrussSolverTests PRIVATE LibTrussDocument)
target_link_libraries(LibTrussSolverTests PRIVATE LibTrussSolver)

include(CTest)
include(Catch)
catch_discover_tests(LibTrussSolverTests)