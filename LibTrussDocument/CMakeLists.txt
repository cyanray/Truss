cmake_minimum_required(VERSION 3.20)
project(LibTrussDocument VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

option(LibTrussDocument_BUILD_SHARED_LIBS "Build LibTrussDocument as a shared library." OFF)
if(LibTrussDocument_BUILD_SHARED_LIBS)
    set(${LIBRARY_TYPE} SHARED)
else()
    set(${LIBRARY_TYPE} STATIC)
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

option(LibTrussDocument_INSTALL "INSTALL_LibTrussDocument" OFF)
if (LibTrussDocument_INSTALL)

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            LibTrussDocumentConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    install(DIRECTORY include
            DESTINATION ${CMAKE_INSTALL_PREFIX}
            )

    install(TARGETS LibTrussDocument
            EXPORT LibTrussDocumentTargets
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            )

    install(EXPORT LibTrussDocumentTargets
            FILE LibTrussDocumentTargets.cmake
            NAMESPACE Truss::
            DESTINATION lib/cmake/LibTrussDocument
            )

    configure_file(LibTrussDocumentConfig.cmake.in LibTrussDocumentConfig.cmake @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LibTrussDocumentConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/LibTrussDocumentConfigVersion.cmake"
            DESTINATION lib/cmake/LibTrussDocument
            )

endif (LibTrussDocument_INSTALL)