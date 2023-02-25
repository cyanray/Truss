
project(LibTrussDocument)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "cl.exe optional: /utf-8")
endif(MSVC)

option(LIB_TRUSS_DOCUMENT_BUILD_SHARED_LIBS "Build LibTrussDocument as a shared library." OFF)
if(LIB_TRUSS_DOCUMENT_BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
else()
    set(LIBRARY_TYPE STATIC)
endif()

add_library(
        LibTrussDocument
        ${LIBRARY_TYPE}
        include/TrussDocument/Tokenizer.hpp src/Tokenizer.cpp
        include/TrussDocument/TrussDocument.hpp src/TrussDocument.cpp
        include/TrussDocument/ParserErrorException.hpp src/ParserErrorException.cpp)

target_include_directories(LibTrussDocument PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(LibTrussDocument PRIVATE magic_enum::magic_enum)