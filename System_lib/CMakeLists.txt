project(System_lib)

set(HEADER_FILES
        SystemOpt.h io_units.h)
set(SOURCE_FILES
        SystemOpt.cpp)

# add lib
add_library(System_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})