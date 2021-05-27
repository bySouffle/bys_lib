# project_lib name
project(TypeConversion_lib)

set(HEADER_FILES
        IntOpt.h
        StringOpt.h
        ArrOpt.h
        )

set(SOURCE_FILES
        IntOpt.cpp
        StringOpt.cpp
        ArrOpt.cpp
        )

# add lib
add_library(TypeConversion_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})