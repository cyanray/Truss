
project(LibTrussSolver)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "cl.exe optional: /utf-8")   
endif(MSVC)

option(LIB_TRUSS_SOLVER_BUILD_SHARED_LIBS "Build LibTrussSolver as a shared library." OFF)
if(LIB_TRUSS_SOLVER_BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

find_package(Eigen3 CONFIG REQUIRED)

add_library(LibTrussSolver ${LIBRARY_TYPE}
            include/Truss/Solver.hpp
            include/Truss/Common/Constants.hpp
            include/Truss/Common/DegreeOfFreedom.hpp
            include/Truss/Common/IComponent.hpp
            include/Truss/Common/Node.hpp
            include/Truss/Common/Resources.hpp
            include/Truss/Constraint/ConstraintBase.hpp
            include/Truss/Constraint/NodeDisplacement.hpp
            include/Truss/Element/Bar.hpp
            include/Truss/Element/ElementBase.hpp
            include/Truss/Load/LoadBase.hpp
            include/Truss/Load/NodeForce.hpp
            include/Truss/Material/MaterialBase.hpp
            include/Truss/Material/Elastic.hpp
            include/Truss/Section/SectionBase.hpp
            include/Truss/Section/Section_Bar.hpp
            include/Truss/Serializer/Serializers.hpp
            include/Truss/Utils/FunctionPointer.hpp
            include/Truss/Utils/SimpleReflection.hpp
            src/Solver.cpp
            src/Bar.cpp
            src/Serializers.cpp
            src/SimpleReflection.cpp
            src/MaterialBase.cpp
            src/ConstraintBase.cpp
            src/LoadBase.cpp
            src/SectionBase.cpp
            src/ElementBase.cpp 
            src/Section_Bar.cpp src/Beam.cpp include/Truss/Element/Beam.hpp src/Section_Beam.cpp include/Truss/Section/Section_Beam.hpp)

target_include_directories(LibTrussSolver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(LibTrussSolver PUBLIC Eigen3::Eigen)
target_link_libraries(LibTrussSolver PUBLIC LibTrussDocument)