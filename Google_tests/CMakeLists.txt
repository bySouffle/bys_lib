# subproject name
project(Google_tests)

# google test sources
add_subdirectory(lib)

set(gtest_SOURCE_DIR /lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#add_executable(Google_Tests_run test1.cpp tests2.cpp)
target_link_libraries(Google_Tests_run DateConverter_lib)
add_executable(Google_Tests_run AbsoluteDateTest.cpp ConverterTests.cpp )

target_link_libraries(Google_Tests_run gtest gtest_main)
