cmake_minimum_required(VERSION 3.0.0)

project(3dmath VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(3dmath INTERFACE)
target_include_directories(
    3dmath INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/3dmath>")

enable_testing()
add_subdirectory(tests)
