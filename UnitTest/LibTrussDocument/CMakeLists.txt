cmake_minimum_required(VERSION 3.20)

find_package(Catch2 3 REQUIRED)

add_executable(LibTrussDocumentTests test.cpp)
target_link_libraries(LibTrussDocumentTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(LibTrussDocumentTests PRIVATE LibTrussDocument)

include(CTest)
include(Catch)
catch_discover_tests(LibTrussDocumentTests)