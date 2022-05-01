# subproject name
project(Google_tests)

# google test sources

set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(HEADER AbsoluteDateTest.h ConverterTests.h)
set(SOURCE AbsoluteDateTest.cpp ConverterTests.cpp)

#add_executable(Google_Tests_run test1.cpp tests2.cpp)
add_executable(Google_tests ${HEADER} ${SOURCE} )

target_link_libraries(Google_tests gtest gtest_main)
