cmake_minimum_required(VERSION 3.20)
project(TrussCLI VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(TrussCLI "src/TrussCLI.cpp" include/UTF8CodePage.hpp)
target_include_directories(TrussCLI PRIVATE "include")

target_link_libraries(TrussCLI PRIVATE LibTrussDocument)
target_link_libraries(TrussCLI PRIVATE LibTrussSolver)

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

option(TrussCLI_INSTALL "INSTALL_TrussCLI" ON)
if (TrussCLI_INSTALL)
    install(TARGETS TrussCLI
            RUNTIME DESTINATION bin
            )

    install(DIRECTORY example_truss
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
            )
endif()