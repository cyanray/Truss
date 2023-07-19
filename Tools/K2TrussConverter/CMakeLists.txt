cmake_minimum_required(VERSION 3.20)
project(K2Truss VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(SOURCES "K2Truss.cpp" "KDoc.cpp" "KDoc.hpp" K2Truss.hpp main.cpp)

add_executable(K2Truss ${SOURCES} ${BACKWARD_ENABLE})
target_include_directories(K2Truss PRIVATE "include")

target_link_libraries(K2Truss PRIVATE LibTrussDocument)
target_link_libraries(K2Truss PRIVATE LibTrussSolver)

add_custom_command(TARGET K2Truss POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/examples
        ${CMAKE_CURRENT_BINARY_DIR}/examples)