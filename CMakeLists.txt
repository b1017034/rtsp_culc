cmake_minimum_required(VERSION 3.16)
project(rtsp_culc C CXX)

set(CMAKE_C_STANDARD 99)
find_package(Threads REQUIRED)
add_executable(rtsp_culc main.cpp RTSP.cpp RTSP.h Network.cpp Network.h util.cpp RTP.cpp RTP.h H264.cpp H264.h RTCP.cpp RTCP.h)

add_executable(socketreplay SocketReplay.cpp SocketReplay.h RTSP.cpp RTSP.h Network.cpp Network.h util.cpp RTP.cpp RTP.h H264.cpp H264.h RTCP.cpp RTCP.h)
TARGET_LINK_LIBRARIES (socketreplay ${CMAKE_THREAD_LIBS_INIT} )

add_executable(test test.cpp)
TARGET_LINK_LIBRARIES (test ${CMAKE_THREAD_LIBS_INIT} )
