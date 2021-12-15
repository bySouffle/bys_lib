# project_lib name
project(DataBase_lib)

set(HEADER_FILES RedisBaseTool.h
        )
set(SOURCE_FILES RedisBaseTool.cpp
        RedisStringTool.cpp RedisStringTool.h)

# add lib
add_library(DataBase_lib STATIC ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries(DataBase_lib hiredis)