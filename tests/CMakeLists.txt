# get gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Build tests in debug mode
add_compile_options(-g -DLLDB)

# All c++ sources in the test directory are considered to be tests
file(GLOB allTestSources *.cpp)

# Put test executables in a separate sub-directory so they can be kept out of IDE
set(testBinDirectory ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
file(MAKE_DIRECTORY ${testBinDirectory})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${testBinDirectory})

# Convenience target to build all tests
add_custom_target(allTests COMMAND ctest --output-on-failure ${CMAKE_SOURCE_DIR}/testBin)

# Build all tests. Create a separate build target for each test and enable google test discovery
foreach(test ${allTestSources})
    get_filename_component(testTarget ${test} NAME_WE)
    add_executable(${testTarget} ${test})
    target_include_directories(${testTarget} PRIVATE ../include)
    target_link_libraries(${testTarget} gtest_main gmock_main)
    gtest_discover_tests(${testTarget})
    add_dependencies(allTests ${testTarget})
endforeach()
