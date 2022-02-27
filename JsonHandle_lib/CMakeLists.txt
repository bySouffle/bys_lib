# project_lib name
project(JsonHandle_lib)

set(HEADER_FILES Archiver/archiver.h
                 Capitalize/capitalize.h
                 Condense/condense.h
                 Filter/filterkey.h)
set(SOURCE_FILES Archiver/archiver.cpp Archiver/archiver_obj.h
        Archiver/PtzTaskJsonReader.cpp Archiver/PtzTaskJsonReader.h
        Archiver/mqtt_struct_ptz.h Archiver/mqtt_struct_switch.h
        Archiver/mqtt_struct_task.h Archiver/mqtt_struct_task.cpp)



include_directories(rapidjson)

# add lib
add_library(JsonHandle_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})