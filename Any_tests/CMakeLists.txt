# project name
project(Any_tests)

#add_subdirectory(/googletest-1.10.x)
# gtest source
set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test code
set(TEST_SOURCES AnyTest.cpp TempClass.h TempClass.cpp Buffer.cpp Buffer.h JsonDealClass.cpp JsonDealClass.h
        RedisTool.cpp RedisTool.h cJSON.h cJSON.cpp)
# adding the gtest_run target
add_executable(Any_tests_run ${TEST_SOURCES})


# linking File_lib_tests_run with lib which will be tested
#set(TEST_LIB /File_lib)
target_link_libraries(Any_tests_run gtest gtest_main hiredis JsonHandle_lib)