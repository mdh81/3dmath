cmake_minimum_required(VERSION 3.24.2)

project(3dmath_python VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.13.6
)
FetchContent_MakeAvailable(pybind11)

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
include_directories(
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/3dmath
)

file(GLOB bindingSrcs "./math3d/*.cpp")
pybind11_add_module(math3d MODULE ${bindingSrcs})
set_target_properties(math3d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/math3d)