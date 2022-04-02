# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(Qt_lib)

find_package(Qt5Widgets REQUIRED)

# Add OpenCV headers location to your include paths
#include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(HEADER_FILES bys_qt_irc.h)
set(SOURCE_FILES bys_qt_irc.cpp)


add_library(Qt_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Qt_lib Qt5::Widgets)
