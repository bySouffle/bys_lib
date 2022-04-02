# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(Opencv_lib)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

# Declare the executable target built from your sources
#add_executable(opencvTest cv_obj_tracking.h cv_obj_tracking.cpp main.cpp )

# Link your application with OpenCV libraries
#target_link_libraries(opencv_lib ${OpenCV_LIBS})

set(HEADER_FILES bys_cv_irc.h)
set(SOURCE_FILES bys_cv_irc.cpp)


add_library(Opencv_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Opencv_lib ${OpenCV_LIBS})
