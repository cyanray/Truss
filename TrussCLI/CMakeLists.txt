cmake_minimum_required(VERSION 3.20)
project(TrussCLI VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

## FetchContent
include(FetchContent)

FetchContent_Declare(backward
        GIT_REPOSITORY https://github.com/bombela/backward-cpp
        GIT_TAG v1.6)
FetchContent_MakeAvailable(backward)

## Target: TrussCLI
set(SOURCES "src/TrussCLI.cpp" "include/UTF8CodePage.hpp" include/Csv/CsvWriter.cpp include/Csv/CsvWriter.hpp include/csv_to_string.hpp include/CsvHeaders.hpp include/Csv/CsvToString.hpp)
add_executable(TrussCLI ${SOURCES} ${BACKWARD_ENABLE})
target_include_directories(TrussCLI PRIVATE "include")

target_link_libraries(TrussCLI PRIVATE LibTrussDocument)
target_link_libraries(TrussCLI PRIVATE LibTrussSolver)
add_backward(TrussCLI)

## POST_BUILD
add_custom_command(TARGET TrussCLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/example_truss
        ${CMAKE_CURRENT_BINARY_DIR}/example_truss)

add_custom_command(TARGET TrussCLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LibTrussDocument>
        $<TARGET_FILE_DIR:TrussCLI>)

add_custom_command(TARGET TrussCLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LibTrussSolver>
        $<TARGET_FILE_DIR:TrussCLI>)

## Install
option(TrussCLI_INSTALL "INSTALL_TrussCLI" ON)
if (TrussCLI_INSTALL)
    install(TARGETS TrussCLI
            RUNTIME DESTINATION bin)

    install(DIRECTORY example_truss
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif ()