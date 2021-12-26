# project_lib name
project(JsonHandle_lib)

set(HEADER_FILES  archiver/archiver.h  )
set(SOURCE_FILES  archiver/archiver.cpp archiver/archiver_obj.h)

include_directories(rapidjson)

# add lib
add_library(JsonHandle_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})