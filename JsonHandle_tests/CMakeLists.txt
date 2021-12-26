# project name
project(JsonHandle_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES archiver_tests.cpp )
# adding the gtest_run target
add_executable(JsonHandle_tests_run ${TEST_SOURCES})

# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /NetWork_lib)
target_link_libraries(JsonHandle_tests_run JsonHandle_lib)

target_link_libraries(JsonHandle_tests_run gtest gtest_main)