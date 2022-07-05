# project name
project(OpenPano_tests)
# gtest source

set(gtest_SOURCE_DIR /googletest-1.10.x/googletest)
include_directories(${gtest_SOURCE_DIR}/include)
include_directories(third_party)
include_directories(third_party/lodepng)
include_directories(third_party/flann)

#add_subdirectory( ${gtest_SOURCE_DIR})
# test code
set(TEST_SOURCES OpenPano_tests.cpp lib/mat.h lib/debugutils.cpp lib/debugutils.h
        lib/utils.cpp lib/utils.h lib/matrix.h lib/matrix.cpp lib/timer.cpp lib/timer.h
        lib/geometry.cpp lib/geometry.h lib/color.cpp lib/color.h
        lib/config.cpp lib/config.h lib/imgio.cpp lib/imgio.h lib/imgproc.cpp lib/imgproc.h lib/polygon.cpp lib/polygon.h lib/planedrawer.cpp lib/planedrawer.h feature/dist.cpp feature/dist.h)
# adding the gtest_run target

add_subdirectory(third_party)
add_executable(OpenPano_tests ${TEST_SOURCES} )
add_compile_options(-fno-elide-constructors)


find_package(Eigen3 REQUIRED)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

add_compile_options(-fno-elide-constructors)
target_link_libraries(OpenPano_tests gtest gtest_main jpeg)
#target_link_libraries(OpenPano_tests lodepng flann pybind11)
target_link_libraries(OpenPano_tests lodepng)