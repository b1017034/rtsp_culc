cmake_minimum_required(VERSION 3.19)
project(rtsp_culc C)

set(CMAKE_C_STANDARD 99)

include_directories(librtsp/include)
add_executable(rtsp_culc main.c)
target_link_libraries(rtsp_culc librtsp/src)