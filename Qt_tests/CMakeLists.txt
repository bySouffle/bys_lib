# project name
project(Qt_lib_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES QtTest.cpp)
# adding the gtest_run target
add_executable(Qt_lib_tests_run ${TEST_SOURCES})

# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /NetWork_lib)
target_link_libraries(Qt_lib_tests_run Qt_lib)

target_link_libraries(Qt_lib_tests_run gtest gtest_main)