# project name
project(DesignPatterns_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES DesignPatternsTests.cpp behavioral_status.cpp behavioral_status.h)
# adding the gtest_run target
add_executable(DesignPatterns_tests_run ${TEST_SOURCES})


# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /File_lib)
target_link_libraries(DesignPatterns_tests_run DesignPatterns_lib)

target_link_libraries(DesignPatterns_tests_run gtest gtest_main)