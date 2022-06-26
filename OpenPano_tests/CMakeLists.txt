# project name
project(OpenPano_tests)
# gtest source

set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include)


#add_subdirectory( ${gtest_SOURCE_DIR})
# test code
set(TEST_SOURCES OpenPano_tests.cpp lib/mat.h lib/debugutils.cpp lib/debugutils.h lib/utils.cpp lib/utils.h)
# adding the gtest_run target
add_executable(OpenPano_tests ${TEST_SOURCES} )
add_compile_options(-fno-elide-constructors)

target_link_libraries(OpenPano_tests gtest gtest_main)