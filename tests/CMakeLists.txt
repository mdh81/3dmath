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
target_include_directories(matrix_test PRIVATE ../include)
target_link_libraries(matrix_test gtest_main)

# Enable test discovery for each test
include(GoogleTest)
gtest_discover_tests(matrix_test)
