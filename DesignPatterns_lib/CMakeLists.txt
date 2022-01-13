# project_lib name
project(DesignPatterns_lib)


set(HEADER_FILES behavioral_patterns/Status.h)
set(SOURCE_FILES behavioral_patterns/Status.cpp)

# add lib
add_library(DesignPatterns_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})