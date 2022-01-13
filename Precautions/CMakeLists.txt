# project name
project(Precautions)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES ./notice.cpp)
# adding the gtest_run target
add_executable(Precautions ${TEST_SOURCES})

# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /NetWork_lib)

target_link_libraries(Precautions gtest gtest_main)