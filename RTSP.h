//
// Created by alpine on 2021/06/23.
//

#ifndef CALC_RTSP_RTSP_H
#define CALC_RTSP_RTSP_H

#ifdef _WIN_32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string>
#include <utility>
#include <vector>

#include "RTP.h"
#include "RTCP.h"
#include "H264.h"

struct SessionInfo {
    /* RTSP protocol stuff */
    unsigned int packetization; /* Packetization mode from SDP data */
    unsigned int clientPortFrom;    /* client_port from */
    unsigned int clientPortTo;      /* client port to   */
    unsigned int serverPortFrom;    /* server port from */
    unsigned int serverPortTo;      /* server port to   */
    std::string  sessionId;    /* session ID       */
};

struct InterleavedFrame {
    /* RTSP interleaved header staff */
    int magic;
    int channel;
    unsigned int rtpLength;
};

class RTSP {
public:
    RTSP(int sock, std::string stream, bool is_dump);
    RTSP(int sock, bool is_dump);
    RTSP(bool is_dump);
    //member variable
    std::string sprop;

    //member functions
    int cmdOptions();
    int cmdSetup();
    int cmdDescribe();
    int cmdPlay();
    int rtspLoop();
    int decode(std::vector<unsigned char> raw, int raw_length);

private:
    //member variable
    int sequence = 0;
    int sock;
    int serverSock;
    std::string stream;
    std::string dsp;
    std::vector<unsigned char> tmpBuffer;
    bool debug =  true;

    RTP *rtp;
    RTCP *rtcp;
    H264 *h264;
    SessionInfo sessionInfo{};
    InterleavedFrame interleavedFrame{};

    //member functions
    void sequenceIncrease();
    int responseStatus(char *response, char **error);
    int decodeInterleavedFrame(std::vector<unsigned char> buffer, int buffer_offset, int buffer_length);

    void sendRTCPRR();
};

#define PROTOCOL_PREFIX   "RTSP://"
#define RTSP_PORT         554
#define RTSP_CLIENT_PORT  9500
#define RTSP_RESPONSE     "RTSP/1.0 "
#define CMD_OPTIONS       "OPTIONS RTSP://%s:%lu RTSP/1.0\r\nCSeq: %i\r\n\r\n"
#define CMD_DESCRIBE      "DESCRIBE %s RTSP/1.0\r\nCSeq: %i\r\nAccept: application/sdp\r\n\r\n"
//#define CMD_SETUP         "SETUP %s  RTSP/1.0\r\nCSeq: %i\r\nTransport: RTP/AVP;unicast;client_port=54984-54985\r\n\r\n"
//#define CMD_SETUP         "SETUP %s  RTSP/1.0\r\nCSeq: %i\r\nTransport: RTP/AVP;unicast;client_port=%i-%i\r\n\r\n"
#define CMD_SETUP         "SETUP %s RTSP/1.0\r\nCSeq: %i\r\nTransport: RTP/AVP/TCP;interleaved=0-1;\r\n\r\n"
#define CMD_PLAY          "PLAY %s RTSP/1.0\r\nCSeq: %i\r\nSession: %s\r\nRange: npt=0.00-\r\n\r\n"

/* Transport header constants */
#define SETUP_SESSION      "Session: "
#define SETUP_TRNS_CLIENT  "client_port="
#define SETUP_TRNS_SERVER  "server_port="

#endif //CALC_RTSP_RTSP_H
