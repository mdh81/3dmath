# get gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_executable(
    matrix_test.test
    matrix_test.cpp)

add_executable(
    vector_test.test
    vector_test.cpp)

add_executable(
    line_test.test
    line_test.cpp)

add_executable(
    types_test.test
    types_test.cpp)

add_executable(
    convex_primitive_test.test
    convex_primitive_test.cpp
)

add_executable(
    sphere_test.test
    sphere_test.cpp
)

add_executable(
    polar_coordinates_test.test
    polar_coordinates_test.cpp
)

add_executable(
    ray_test.test
    ray_test.cpp
)

add_executable(
    plane_test.test
    plane_test.cpp
)

set(allTests
    matrix_test.test
    vector_test.test
    line_test.test
    types_test.test
    sphere_test.test
    convex_primitive_test.test
    polar_coordinates_test.test
    ray_test.test
    plane_test.test)

foreach (test ${allTests})
    target_include_directories(
        ${test}
        PRIVATE ../include
    )
    target_link_libraries(
        ${test}
        gtest_main
        gmock_main
    )
endforeach(test)

# Enable test discovery for each test
include(GoogleTest)
foreach (test ${allTests})
    gtest_discover_tests(${test} PROPERTIES ENVIRONMENT "baselineDir=${CMAKE_SOURCE_DIR}/tests/baseline")
endforeach(test)
