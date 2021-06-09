cmake_minimum_required(VERSION 3.19)
project(rtsp_culc C)

set(CMAKE_C_STANDARD 99)

include_directories(${rtsp_culc_SOURCE_DIR}/librtsp/include)
#include_directories(${rtsp_culc_SOURCE_DIR}/libpomp/include)
include_directories(${rtsp_culc_SOURCE_DIR}/libfutils/include)
include_directories(${rtsp_culc_SOURCE_DIR}/ulog/libulog/include)

#link_directories(/librtsp/src)
#link_directories(${rtsp_culc_SOURCE_DIR}/libpomp/src)
add_library(libpomp
        SHARED
        libpomp/src/pomp_addr.c
        libpomp/src/pomp_buffer.c
        libpomp/src/pomp_buffer.h
        libpomp/src/pomp_config.h
        libpomp/src/pomp_conn.c
        libpomp/src/pomp_ctx.c
        libpomp/src/pomp_decoder.c
        libpomp/src/pomp_encoder.c
        libpomp/src/pomp_evt.c
        libpomp/src/pomp_evt.h
        libpomp/src/pomp_list.h
        libpomp/src/pomp_log.c
        libpomp/src/pomp_log.h
        libpomp/src/pomp_loop.c
        libpomp/src/pomp_loop.h
        libpomp/src/pomp_msg.c
        libpomp/src/pomp_priv.h
        libpomp/src/pomp_prot.c
        libpomp/src/pomp_prot.h
        libpomp/src/pomp_timer.c
        libpomp/src/pomp_timer.h
        libpomp/src/pomp_watchdog.c
        libpomp/src/pomp_watchdog.h
        libpomp/src/sys_eventfd.h
        libpomp/src/sys_timerfd.h
        )

target_include_directories(libpomp
        PUBLIC libpomp/include
        )

file(GLOB LIBFUTILS_SOURCE libfutils/src/*)
add_library(libfutils
        SHARED
        ${LIBFUTILS_SOURCE})

target_include_directories(libfutils
        PUBLIC libfutils/include)

file(GLOB LIBULOG_SOURCE ulog/libulog/*.h ulog/libulog/*.c)
add_library(libulog
        SHARED
        ${LIBULOG_SOURCE}
        )
target_include_directories(libulog
        PUBLIC ulog/libulog/include)

file(GLOB LIBSRTP_SOURCE librtsp/src/*.h librtsp/src/*.c)
add_library(librtsp
        SHARED
        ${LIBSRTP_SOURCE}
)



target_include_directories(librtsp
        PUBLIC librtsp/include
        )


target_link_libraries(librtsp libpomp libfutils)
target_link_libraries(libpomp libulog)


add_executable(rtsp_culc main.c)
target_link_libraries(rtsp_culc librtsp)