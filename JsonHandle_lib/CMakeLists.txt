# project_lib name
project(JsonHandle_lib)

set(HEADER_FILES Archiver/archiver.h
                 Capitalize/capitalize.h
                 Condense/condense.h)
set(SOURCE_FILES Archiver/archiver.cpp Archiver/archiver_obj.h
        Archiver/PtzTaskJsonReader.cpp Archiver/PtzTaskJsonReader.h)



include_directories(rapidjson)

# add lib
add_library(JsonHandle_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})