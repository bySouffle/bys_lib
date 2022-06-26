# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(Qt_lib)

find_package(Qt5Widgets REQUIRED)
#find_package(Qt5 COMPONENTS Core Gui Qml Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 由于生成的C++文件都是放在编译目录里的（在Shadow Building中有别于源码目录），
# 所以还需要将编译目录包含到Include目录，否则编译器会找不到这些头文件：
#set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(HEADER_FILES bys_qt_irc.h)
set(SOURCE_FILES bys_qt_irc.cpp)


add_library(Qt_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Qt_lib Qt5::Widgets)
