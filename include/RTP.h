//
// Created by alpine on 2021/06/24.
//

#ifndef CALC_RTSP_RTP_H
#define CALC_RTSP_RTP_H

#include <cstdint>
#include <ctime>
#include <vector>
#include <fstream>

#ifndef _WIN32
#include <sys/time.h>
#endif

#include "H264.h"
#include "RTCP.h"

#define RTP_FREQ    90000
#define RTP_SPROP   "sprop-parameter-sets="
#define MAX_BUF_SIZE 10000

class RTP {
    struct Header {
        unsigned int version: 2;     /* protocol version */
        unsigned int padding: 1;     /* padding flag */
        unsigned int extension: 1;   /* header extension flag */
        unsigned int cc: 4;          /* CSRC count */
        unsigned int marker: 1;      /* marker bit */
        unsigned int pt: 7;          /* payload type */
        uint16_t seq: 16;            /* sequence number */
        uint32_t ts;                /* timestamp */
        uint32_t ssrc;              /* synchronization source */
        uint32_t csrc[1];           /* optional CSRC list */
    };

    struct Stats {
        uint16_t firstSeq;         /* first sequence                   */
        uint16_t highestSeq;       /* highest sequence                 */
        uint16_t rtpReceived;      /* RTP sequence number received     */
        uint32_t rtpIdentifier;    /* source identifier                */
        uint32_t rtpTs;            /* RTP timestamp                    */
        uint32_t rtpCumLost;       /* RTP cumulative packet lost       */
        uint32_t rtpExpectedPrior; /* RTP expected prior               */
        uint32_t rtpReceivedPrior; /* RTP received prior               */
        double transit;          /* Transit time. RFC3550 A.8        */
        double jitter;           /* Jitter                           */
        double delay;            /* delay                            */
        double frameDelay;
        uint32_t frames;
        uint32_t frameCount = 0;
        uint32_t lst;
        uint32_t lastDlsr;         /* Last DLSR                        */
        uint32_t lastRcvSrTs;      /* Last arrival in RTP format       */
        uint32_t delaySncLastSr;   /* Delay sinse last SR              */
        struct timeval lastRcvSrTime;           /* Last SR arrival                  */
        struct timeval lastRcvTime;
        double rttFrac;
    };

public:
    explicit RTP(const H264& h264Dec, RTCP *rtcpDec) : h264(h264Dec), rtcp(rtcpDec) {
      this->outFile.open("rtp.csv", std::ios::out);
      this->outFrameFile.open("rtp_frame.csv", std::ios::out);
      this->outFile << "RTP Received,Sequence,RTP Timestamp,Jitter (ms),Delay (ms)" << std::endl;
      this->outFrameFile << "Frame Received,Delay" << std::endl;
    }

    unsigned int decode(std::vector<unsigned char> raw);

    Header header{};
    Stats stats{};

private:
    H264 h264;
    RTCP *rtcp;
    std::fstream outFile;
    std::fstream outFrameFile;
    std::vector<unsigned char > payload;
    bool debug = false;

    void statsUpdate();
    void printStats();
    void printHeader();
    void outputStats();
    void outputFrameStats();
};




#endif //CALC_RTSP_RTP_H
