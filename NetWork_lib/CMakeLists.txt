# project_lib name
project(NetWork_lib)

set(HEADER_FILES
        TcpOpt.h
        UdpOpt.h
        NetBase.h
        PipeOpt.h
        )
set(SOURCE_FILES
        TcpOpt.cpp
        UdpOpt.cpp
        NetBase.cpp
        PipeOpt.cpp
        )

# add lib
add_library(NetWork_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})