# project_lib name
project(File_lib)

set(HEADER_FILES
        FileOpt.h)
set(SOURCE_FILES
        FileOpt.cpp)

# add lib
add_library(File_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})