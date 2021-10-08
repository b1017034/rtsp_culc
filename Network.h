//
// Created by alpine on 2021/06/23.
//

#ifndef CALC_RTSP_NETWORK_H
#define CALC_RTSP_NETWORK_H
#include <string>
#ifdef _WIN_32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#endif

class Network{
public:
    static int tcpConnect(std::string host, unsigned short port);
};


#endif //CALC_RTSP_NETWORK_H
