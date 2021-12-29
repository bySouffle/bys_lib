project(System_lib)

set(HEADER_FILES
        SystemOpt.h)
set(SOURCE_FILES
        SystemOpt.cpp SystemMonitor.cpp SystemMonitor.h ThreadOutSysInfo.cpp ThreadOutSysInfo.h)

set(baseLib_SOURCE_DIR /BaseLib)
include_directories(${baseLib_SOURCE_DIR}/rapidjson ${baseLib_SOURCE_DIR})

# add lib
add_library(System_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})