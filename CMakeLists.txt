cmake_minimum_required(VERSION 3.5)

project(3dmath VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INSTALL_GTEST OFF)
# Allow root git modules to skip tests (e.g. MeshViewer disables tests
# for emscripten builds)
if (NOT EMSCRIPTEN)
    set(enableTesting ON)
endif()

add_library(3dmath INTERFACE)
target_include_directories(
    3dmath INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/3dmath>")

if (enableTesting)
    enable_testing()
    add_subdirectory(tests)
endif()
