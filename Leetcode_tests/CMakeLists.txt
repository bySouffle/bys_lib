# project name
project(Leetcode_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


# test code
set(TEST_SOURCES leetcode_tests.cpp )
# adding the gtest_run target
add_executable(Leetcode_tests ${TEST_SOURCES})

# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /NetWork_lib) libhv
target_link_libraries(Leetcode_tests gtest gtest_main )

