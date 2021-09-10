cmake_minimum_required(VERSION 3.19)
project(bys_lib)

set(CMAKE_CXX_STANDARD 14)

# 编译google test生成libtest.a静态库
add_subdirectory(./googletest-1.10.x)

set(mode_file
        NetWork_lib/TcpOpt.cpp NetWork_lib/TcpOpt.h
        TypeConversion_lib/ArrOpt.cpp TypeConversion_lib/ArrOpt.h
        NetWork_lib/UdpOpt.cpp NetWork_lib/UdpOpt.h
        TypeConversion_lib/StringOpt.cpp TypeConversion_lib/StringOpt.h
        TypeConversion_lib/IntOpt.cpp TypeConversion_lib/IntOpt.h
        File_lib/FileOpt.cpp File_lib/FileOpt.h)

add_executable(bys_lib main.cpp ${mode_file})



include_directories(File_lib)
include_directories(NetWork_lib)
include_directories(TypeConversion_lib)
include_directories(System_lib)


add_subdirectory(File_lib)
add_subdirectory(NetWork_lib)
add_subdirectory(TypeConversion_lib)
add_subdirectory(System_lib)

target_link_libraries(bys_lib File_lib NetWork_lib TypeConversion_lib pthread)

add_subdirectory(File_tests)
add_subdirectory(NetWork_tests)
add_subdirectory(TypeConversion_tests)
add_subdirectory(System_tests)
