project(System_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES SystemTest.cpp)

# adding the gtest_run target
add_executable(System_tests_run ${TEST_SOURCES})

# linking File_lib_tests_run with lib which will be tested
target_link_libraries(System_tests_run  System_lib)
target_link_libraries(System_tests_run gtest gtest_main)