cmake_minimum_required(VERSION 3.0.0)
project(thesis-tests VERSION 0.1.0)

add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES tests.cpp)

add_executable(thesis-tests ${TEST_SOURCE_FILES})
target_link_libraries(thesis-tests thesis gtest gtest_main)

add_test(
    NAME thesis-tests
    COMMAND thesis-tests
)