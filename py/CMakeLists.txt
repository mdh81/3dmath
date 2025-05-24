cmake_minimum_required(VERSION 3.24.2)

project(3dmath_python VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PYTHON_FIND_VIRTUALENV ONLY)
find_package(Python 3.13 REQUIRED COMPONENTS Development Interpreter)
if(Python_NOT_FOUND)
    message(FATAL_ERROR "Unable to find python from a virtual environment. Has a virtual environment been defined and activated")
endif()
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
set (PYBIND11_PYTHON_VERSION "3.13")
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.13.6
)
FetchContent_MakeAvailable(pybind11)
message(STATUS "Pybind configured with ${Python3_EXECUTABLE}")

include_directories(
    ${Python_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/3dmath
)

file(GLOB bindingSrcs "./math3d/*.cpp")
pybind11_add_module(math3d MODULE ${bindingSrcs})
set_target_properties(math3d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/math3d)