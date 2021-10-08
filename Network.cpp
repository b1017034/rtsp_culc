//
// Created by alpine on 2021/06/23.
//

#include "Network.h"
#include <iostream>

int Network::tcpConnect(std::string host, unsigned short port) {
#ifdef _WIN32
    WSAData wsaData{};
    WSAStartup(MAKEWORD(2,0), &wsaData);
#endif
    struct sockaddr_in server{};
    int sock;
    char buf[32];


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        std::cout << "Error socket initialized" << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
#ifdef _WIN32
    server.sin_addr.S_un.S_addr = inet_addr(host.c_str());
#else
  struct hostent *servhost;
    servhost = gethostbyname(host.c_str());
    if ( servhost == NULL ){
      fprintf(stderr, "ERROR:gethostbyname(%s)\n", host.c_str());
      exit(EXIT_FAILURE);
    }
    bcopy(servhost->h_addr, &server.sin_addr, servhost->h_length);
#endif

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
#ifdef _WIN32
      closesocket(sock);
      WSACleanup();
#else
      close(sock);
#endif

        std::cout << "Error connecting to " << host << ":" << port << std::endl;

        return -1;
    }

    return sock;
}
