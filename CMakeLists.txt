cmake_minimum_required(VERSION 3.19)
project(bys_lib)

set(CMAKE_CXX_STANDARD 14)

add_executable(bys_lib main.cpp File/FileOpt.cpp File/FileOpt.h cmake-build-debug/Udp_Opt.cpp cmake-build-debug/Udp_Opt.h NetWork/TcpOpt.cpp NetWork/TcpOpt.h TypeConversion/ArrOpt.cpp TypeConversion/ArrOpt.h NetWork/UdpOpt.cpp NetWork/UdpOpt.h TypeConversion/StringOpt.cpp TypeConversion/StringOpt.h TypeConversion/IntOpt.cpp TypeConversion/IntOpt.h)