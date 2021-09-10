# project_lib name
project(NetWork_lib)

set(HEADER_FILES
        TcpOpt.h
        UdpOpt.h
        NetBase.h
        PipeOpt.h
        dsp_socket.h
        dsp_udpsocket.h
        EpollServer.h)
set(SOURCE_FILES
        TcpOpt.cpp
        UdpOpt.cpp
        NetBase.cpp
        PipeOpt.cpp
        dsp_socket.cpp
        dsp_udpsocket.cpp
        EpollServer.cpp ScalableBuff.cpp ScalableBuff.h)

# add lib
add_library(NetWork_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})