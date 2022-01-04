//
// Created by alpine on 2021/06/24.
//

#include <cstring>
#include "include/RTP.h"

#include "util.cpp"
#include <time.h>

unsigned int RTP::decode(std::vector<unsigned char> raw) {
    unsigned int i = 0;
    int payloadSize;
    this->header.version = raw[i] >> 6;
    this->header.padding = (raw[i] & 0x20) >> 5;
    this->header.extension = (raw[i] & 0x10) >> 4;
    this->header.cc = (raw[i] & 0x0f);

    i++;
    this->header.marker = raw[i] >> 7;
    this->header.pt = raw[i] & 0x7f;

    i++;
    this->header.seq = raw[i] * 256 + raw[i + 1];

    i += 2;
    this->header.ts = (
            raw[i]     << 24 |
            raw[i + 1] << 16 |
            raw[i + 2] << 8  |
            raw[i + 3]);

    i += 4;
    this->header.ssrc = (
            raw[i]     << 24 |
            raw[i + 1] << 16 |
            raw[i + 2] << 8  |
            raw[i + 3]);
    this->stats.rtpIdentifier = this->header.ssrc;

    i += 4;
    payloadSize = raw.size() - i;
    this->payload = { raw.begin() + i, raw.end() };
    this->printHeader();
    this->statsUpdate();

    this->h264.decode(this->payload);
    i += payloadSize;
    return i;
}

/* Every time a RTP packet arrive, update the stats */
void RTP::statsUpdate(){
    double transit;
    double delta;
    struct timeval now;
#ifdef _WIN32
    mingw_gettimeofday(&now,NULL);
#else
    gettimeofday(&now, NULL);
#endif

    this->stats.rtpReceived++;

    /* Highest sequence */
    if (this->header.seq > this->stats.highestSeq) {
        this->stats.highestSeq = this->header.seq;
    }

    /* Update RTP timestamp */
    if (this->stats.lastRcvTime.tv_sec == 0) {
        this->stats.firstSeq = this->header.seq;
#ifdef _WIN32
        mingw_gettimeofday(&now,NULL);
#else
        gettimeofday(&now, NULL);
#endif
    }

    /* delay */
    int incUnit = this->header.ts - this->rtcp->stats.lastRtpTimestamp;
    if (incUnit < 0) incUnit = -incUnit;

    int timeDiffHigh = incUnit / 90000;
    uint32_t timeDiffLow = incUnit % 90000;
    timeDiffLow = (uint32_t)(timeDiffLow * 1e+6 / 90000);
    double delay;
    timeval sendTime = this->rtcp->stats.lastSRTimestamp;
    sendTime.tv_sec += timeDiffHigh;
    sendTime.tv_usec += timeDiffLow;

#ifdef _WIN32
    mingw_gettimeofday(&now,NULL);
#else
    gettimeofday(&now, NULL);
#endif
    delay = ( now.tv_sec - sendTime.tv_sec ) +
            (( now.tv_usec - sendTime.tv_usec ) * 1e-6);
//    delay = delay * 1000;
    this->stats.delay =  delay;

    /* Jitter */
    transit = this->stats.delay;
    if (debug) std::cout << format("       Transit     : %lf", transit) << std::endl;
    delta = transit - this->stats.transit;
    this->stats.transit = transit;
    if (delta < 0) delta = -delta;
    this->stats.jitter += ((1.0/16.0) * (((double) delta - this->stats.jitter)) * 1000) / 1000;

    printf("   RTP Received    : %u\n", this->stats.rtpReceived);
    std::cout << format("       timestamp         : %i", this->header.ts) << std::endl;
    std::cout << format("       Jitter (ms)       : %lf", this->stats.jitter) << std::endl;
    std::cout << format("       Delay  (ms)       : %lf", this->stats.delay ) << std::endl;
    if (this->stats.rtpTs == this->header.ts) {
      this->stats.frameDelay += this->stats.delay;
      this->stats.frames += 1;
    } else {
      this->stats.frameCount += 1;
      this->stats.frameDelay = this->stats.frameDelay / this->stats.frames;
      std::cout << format("       Frame Delay  (ms) : %lf", this->stats.frameDelay ) << std::endl;
      this->outputFrameStats();
      this->stats.frameDelay = 0.0;
      this->stats.frames = 0;

    }
    this->stats.rtpTs = this->header.ts;



    /* print the new stats */
    this->printStats();
    this->outputStats();
}

void RTP::printHeader() {
    if (debug) {
        std::cout << ">> RTP Header" << std::endl;
        std::cout << format("       Version     : %i", this->header.version) << std::endl;
        std::cout << format("       Padding     : %i", this->header.padding) << std::endl;
        std::cout << format("       Extension   : %i", this->header.extension) << std::endl;
        std::cout << format("       CSRC Count  : %i", this->header.cc) << std::endl;
        std::cout << format("       Marker      : %i", this->header.marker) << std::endl;
        std::cout << format("       Payload Type: %i", this->header.pt) << std::endl;
        std::cout << format("       Sequence    : %i", this->header.seq) << std::endl;
        std::cout << format("       Timestamp   : %lu", this->header.ts) << std::endl;
        std::cout << format("       Sync Source : %lu", this->header.ssrc) << std::endl;
    }

}

void RTP::printStats() {
    if(debug) {
        std::cout << ">> RTP Stats" << std::endl;
        printf("   First Sequence  : %u\n", this->stats.firstSeq);
        printf("   Highest Sequence: %u\n", this->stats.highestSeq);
        printf("   RTP Received    : %u\n", this->stats.rtpReceived);
        printf("   RTP Identifier  : %u\n", this->stats.rtpIdentifier);
        printf("   RTP Timestamp   : %u\n", this->stats.rtpTs);
        printf("   Jitter (ms)     : %f\n", this->stats.jitter);
        printf("   Delay  (ms)     : %f\n", this->stats.delay);
        printf("   Last DLSR       : %i\n", this->stats.lastDlsr);
    }

}

void RTP::outputStats() {
  this->outFile << format("%i,%i,%i,%f,%f",
                          this->stats.rtpReceived,
                          this->header.seq,
                          this->stats.rtpTs,
                          this->stats.jitter,
                          this->stats.delay)
                << std::endl;
  }

  void RTP::outputFrameStats() {
    this->outFrameFile << format("%i, %f",
                                 this->stats.frameCount,
                                 this->stats.frameDelay)
                       << std::endl;
  }
