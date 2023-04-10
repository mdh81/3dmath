# get gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_executable(
    matrix_test 
    matrix_test.cpp)

add_executable(
    vector_test 
    vector_test.cpp)

add_executable(
    line_test 
    line_test.cpp)

set(allTests
    matrix_test
    vector_test
    line_test)

foreach (test ${allTests})
    target_include_directories(
        ${test}
        PRIVATE ../include
    )
    target_link_libraries(
        ${test}
        gtest_main
    )
endforeach(test)

# Enable test discovery for each test
include(GoogleTest)
foreach (test ${allTests})
    gtest_discover_tests(${test})
endforeach(test)
