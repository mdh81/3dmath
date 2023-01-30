cmake_minimum_required(VERSION 3.0.0)

project(3dmath VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/3dmath/Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/3dmath/Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/3dmath/Line.h)

add_library(3dmath INTERFACE)
target_include_directories(3dmath INTERFACE "$<BUILD_INTERFACE:{header_files}>")

install(DIRECTORY include/3dmath DESTINATION include)

#TODO: Skip gtests from install rule

enable_testing()
add_subdirectory(tests)
