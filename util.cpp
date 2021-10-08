//
// Created by alpine on 2021/06/23.
//
#include <string>
#include <cstdio>
#include <vector>

template <typename ... Args>
std::string format(const std::string& fmt, Args ... args ) {
    size_t len = std::snprintf( nullptr, 0, fmt.c_str(), args ... );
    std::vector<char> buf(len + 1);
    std::snprintf(&buf[0], len + 1, fmt.c_str(), args ... );
    return std::string(&buf[0], &buf[0] + len);
}

//int gettimeofday(struct timeval * tp, struct timezone * tzp) {
//    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
//    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
//    // until 00:00:00 January 1, 1970
//    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);
//
//    SYSTEMTIME  system_time;
//    FILETIME    file_time;
//    uint64_t    time;
//
//    GetSystemTime( &system_time );
//    SystemTimeToFileTime( &system_time, &file_time );
//    time =  ((uint64_t)file_time.dwLowDateTime )      ;
//    time += ((uint64_t)file_time.dwHighDateTime) << 32;
//
//    tp->tvSec  = (long) ((time - EPOCH) / 10000000L);
//    tp->tvUSec = (long) (system_time.wMilliseconds * 1000);
//    return 0;
//}

/* Check if a bit is 1 or 0 */
#define CHECK_BIT(var, pos) !!((var) & (1 << (pos)))