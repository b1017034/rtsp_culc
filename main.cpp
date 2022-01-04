#include <iostream> //標準入出力

//#include <arpa/inet.h> //バイトオーダの変換に利用
//#include <sys/socket.h> //アドレスドメイン

/* local headers */
#include "include/RTSP.h"
#include "include/Network.h"

#include "util.cpp"

int main(){
    std::string stream = "rtsp://127.0.0.1:8554/sample.mkv";
//    std::string stream = "RTSP://127.0.0.1:8554/test";
    const char *host = "127.0.0.1";
    unsigned int port = 8554;
    unsigned int sock = Network::tcpConnect(host, port);
    bool isDump = true; //is dump to file or not

    RTSP r(sock, stream, isDump);

    int n = r.rtspLoop();

    std::cout << "n: " << n << std::endl;
    return n;
}
