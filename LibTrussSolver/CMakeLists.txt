cmake_minimum_required(VERSION 3.20)
project(LibTrussSolver VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_options("$<$<C_COMPILER_ID:Clang>:-stdlib=libc++>")
add_compile_options("$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>")

option(LibTrussSolver_BUILD_SHARED_LIBS "Build LibTrussSolver as a shared library." OFF)
if (LibTrussSolver_BUILD_SHARED_LIBS)
    set(LIBRARY_TYPE SHARED)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else ()
    set(LIBRARY_TYPE STATIC)
endif ()

find_package(Eigen3 CONFIG REQUIRED)

set(SOURCES
        "include/Truss/Solver.hpp"
        "include/Truss/Common/Constants.hpp"
        "include/Truss/Common/DegreeOfFreedom.hpp"
        "include/Truss/Common/IComponent.hpp"
        "include/Truss/Common/Node.hpp"
        "include/Truss/Common/Resources.hpp"
        "include/Truss/Constraint/ConstraintBase.hpp"
        "include/Truss/Constraint/NodeConstraint.hpp"
        "include/Truss/Element/Bar.hpp"
        "include/Truss/Element/ElementBase.hpp"
        "include/Truss/Load/LoadBase.hpp"
        "include/Truss/Load/NodeLoad.hpp"
        "include/Truss/Load/BeamUniformLoad.hpp"
        "include/Truss/Material/MaterialBase.hpp"
        "include/Truss/Material/Elastic.hpp"
        "include/Truss/Section/SectionBase.hpp"
        "include/Truss/Section/Section_Bar.hpp"
        "include/Truss/Section/Section_CSTriangle.hpp"
        "include/Truss/Serializer/Serializers.hpp"
        "include/Truss/Utils/FunctionPointer.hpp"
        "include/Truss/Utils/SimpleReflection.hpp"
        "include/Truss/Section/Section_Beam.hpp"
        "include/Truss/Element/Beam.hpp"
        "include/Truss/Element/CSTriangle.hpp"
        "src/Solver.cpp"
        "src/Bar.cpp"
        "src/Serializers.cpp"
        "src/SimpleReflection.cpp"
        "src/MaterialBase.cpp"
        "src/ConstraintBase.cpp"
        "src/LoadBase.cpp"
        "src/SectionBase.cpp"
        "src/ElementBase.cpp"
        "src/Section_Bar.cpp"
        "src/Beam.cpp"
        "src/Section_Beam.cpp"
        "src/Node.cpp"
        "src/BeamUniformLoad.cpp"
        "src/CSTriangle.cpp" src/Section_CSTriangle.cpp include/Truss/Set/NodeSet.cpp include/Truss/Set/NodeSet.hpp include/Truss/Set/SetBase.hpp include/Truss/Load/NodeSetLoad.hpp include/Truss/Constraint/NodeSetConstraint.hpp include/Truss/Utils/GaussianQuadrature.hpp include/Truss/Element/Quad4.hpp src/Quad4.cpp include/Truss/Section/Section_CSQuad.hpp src/Section_CSQuad.cpp)


option(LibTrussSolver_DOUBLE_PRECISION "Build LibTrussSolver As Double Precision Version" OFF)
if(LibTrussSolver_DOUBLE_PRECISION)
    target_compile_definitions(LibTrussSolver PRIVATE TRUSS_DOUBLE_PRECISION)
endif()


add_library(LibTrussSolver ${LIBRARY_TYPE} ${SOURCES})
target_link_libraries(LibTrussSolver PUBLIC Eigen3::Eigen)
target_link_libraries(LibTrussSolver PUBLIC LibTrussDocument)

target_include_directories(LibTrussSolver PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

option(LibTrussSolver_DOUBLE_PRECISION "Build LibTrussSolver As Double Precision Version" OFF)
if (LibTrussSolver_DOUBLE_PRECISION)
    target_compile_definitions(LibTrussSolver PRIVATE TRUSS_DOUBLE_PRECISION)
endif ()


option(LibTrussSolver_INSTALL "INSTALL_LibTrussSolver" ON)
if (LibTrussSolver_INSTALL)

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            LibTrussSolverConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    install(DIRECTORY include
            DESTINATION ${CMAKE_INSTALL_PREFIX}
            )

    install(TARGETS LibTrussSolver LibTrussDocument
            EXPORT LibTrussSolverTargets
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            )

    install(EXPORT LibTrussSolverTargets
            FILE LibTrussSolverTargets.cmake
            NAMESPACE Truss::
            DESTINATION cmake/LibTrussSolver
            )

    configure_file(LibTrussSolverConfig.cmake.in LibTrussSolverConfig.cmake @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LibTrussSolverConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/LibTrussSolverConfigVersion.cmake"
            DESTINATION cmake/LibTrussSolver
            )

endif (LibTrussSolver_INSTALL)