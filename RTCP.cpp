//
// Created by alpine on 2021/06/24.
//

#include "include/RTCP.h"
#include "util.cpp"
#include <iostream>

#ifndef _WIN32
#include <sys/time.h>
#endif

void RTCP::decode(std::vector<unsigned char> payload) {
    int i = 0;
    int start = 0;
    while (start < payload.size()) {
        i = start;
        i = this->decodeHeader(payload, i);

        i++;
        switch (this->header.type) {
            case PacketType::SR:
                this->decodeSR(payload, i);
                break;
            case PacketType::SDES:
                this->decodeSDES(payload, i);
                break;
        }
        start += (this->header.length + 1) * 4;
    }

}

int RTCP::decodeHeader(std::vector<unsigned char> payload, int i) {
    /* Decode RTCP Header */
    this->header.version = payload[i] >> 6;
    this->header.padding = (payload[i] & 0x20) >> 5;
    this->header.item_count = (payload[i] & 0x1F);

    /* type */
    i++;
    this->header.type = static_cast<PacketType>(payload[i]);

    /* length */
    i++;
    this->header.length = payload[i];
    i++;
    this->header.length += payload[i];

    this->printHeader();

    return i;
}

int RTCP::decodeSR(std::vector<unsigned char> payload, int i) {
    this->senderReport.sdSsrc = (
            (payload[i + 3]) |
            (payload[i + 2] <<  8) |
            (payload[i + 1] << 16) |
            (payload[i] << 24)
            );

    this->senderReport.tsMsw = (
            (payload[i + 7]) |
            (payload[i + 6] <<  8) |
            (payload[i + 5] << 16) |
            (payload[i + 4] << 24)
            );

    this->senderReport.tsLsw  =  (
            (payload[i + 11]) |
            (payload[i + 10] <<  8) |
            (payload[i +  9] << 16) |
            (payload[i +  8] << 24)
    );

    /* RTP timestamp */
    this->senderReport.tsRtp  =  (
            (payload[i + 15]) |
            (payload[i + 14] <<  8) |
            (payload[i + 13] << 16) |
            (payload[i + 12] << 24)
    );

    this->senderReport.sdPktCount =  (
            (payload[i + 19]) |
            (payload[i + 18] <<  8) |
            (payload[i + 17] << 16) |
            (payload[i + 16] << 24)
    );

    this->senderReport.sdOctCount =  (
            (payload[i + 23]) |
            (payload[i + 22] <<  8) |
            (payload[i + 21] << 16) |
            (payload[i + 20] << 24)
    );

    i += 24;

    this->senderReport.timestamp = ntpToTimeval(this->senderReport.tsMsw, this->senderReport.tsLsw);

    struct timeval now;

#ifdef _WIN32
    mingw_gettimeofday(&now, nullptr);
#else
    gettimeofday(&now, nullptr);
#endif
    this->senderReport.receiveTime = now;

    this->printSR();

    this->stats.lastSRTimestamp = this->senderReport.timestamp;
    this->stats.lastRtpTimestamp = this->senderReport.tsRtp;
    return i;
}

int RTCP::decodeSDES(std::vector<unsigned char> payload, int i) {
    this->sourceDescription.identifier = (
            (payload[i + 3]) |
            (payload[i + 2] <<  8) |
            (payload[i + 1] << 16) |
            (payload[i    ] << 24)
    );

    i += 4;

    do {
        this->sourceDescription.type = static_cast<SDESType>(payload[i]);
        i++;
        switch (this->sourceDescription.type) {
            case SDESType::CNAME:
                this->sourceDescription.length = payload[i];
                /* we skip the source name, we dont need it */
                i += this->sourceDescription.length + 1;
                break;
            case SDESType::TOOL:
                this->sourceDescription.length = payload[i];
                /* we skip the source name, we dont need it */
                i += this->sourceDescription.length + 1;
                break;
            case SDESType::BYE:
                this->sourceDescription.length = payload[i];
                break;
            default:
                break;

        }

    } while (this->sourceDescription.type != SDESType::BYE) ;

    this->printSDES();

    return i;
}

int RTCP::sendReceiverReport() {
    this->culcDelayLastSR();
}

uint32_t RTCP::culcDelayLastSR() {
    struct timeval now{};
    double delay;

    if (this->senderReport.timestamp.tv_sec == 0) return 0;

#ifdef _WIN32
    mingw_gettimeofday(&now,NULL);
#else
    gettimeofday(&now, NULL);
#endif
    delay = ( now.tv_sec - this->senderReport.receiveTime.tv_sec ) +
            (( now.tv_usec - this->senderReport.receiveTime.tv_usec ) * 1e-6);
    delay = (delay * 65536);
    this->stats.delaySncLastSR = (uint32_t) delay;
    return this->stats.delaySncLastSR;
}

void RTCP::printHeader() const {
    if (this->debug) {
        std::cout << ">>RTCP Header" << std::endl;
        std::cout << format("       Version: %i", this->header.version) << std::endl;
        std::cout << format("       Padding: %i", this->header.padding) << std::endl;
        std::cout << format("       Count  : %i", this->header.item_count) << std::endl;
        std::cout << format("       Type   : %i", this->header.type) << std::endl;
        std::cout << format("       Length: %i", this->header.length) << std::endl;
    }

}

void RTCP::printSR() const {
    if (this->debug) {
        std::cout << ">>RTCP SR" << std::endl;
        std::cout << format("       SSRC    : %lu", this->senderReport.sdSsrc) << std::endl;
        std::cout << format("       TS MSW  : %lu", this->senderReport.tsMsw) << std::endl;
        std::cout << format("       TS LSW  : %lu", this->senderReport.tsLsw) << std::endl;
        std::cout << format("       TS RTP  : %lu", this->senderReport.tsRtp) << std::endl;
        std::cout << format("       SD PK CT: %i", this->senderReport.sdPktCount) << std::endl;
        std::cout << format("       SD OC CT: %i", this->senderReport.sdOctCount) << std::endl;
        std::cout << format("       TV SEC  : %i", this->senderReport.tvSec) << std::endl;
        std::cout << format("       TV USEC : %i", this->senderReport.tvUSec) << std::endl;
    }
}

void RTCP::printSDES() {
    if (this->debug) {
        std::cout << ">>RTCP SDES" << std::endl;
        std::cout << format("       Identifier    : %lu", this->sourceDescription.identifier) << std::endl;
        std::cout << "    >>RTCP SDES items" << std::endl;
        std::cout << format("           Type    : %lu", this->sourceDescription.type) << std::endl;
        std::cout << format("           length  : %lu", this->sourceDescription.length) << std::endl;
    }
}

timeval RTCP::ntpToTimeval(uint32_t msw, uint32_t lsw) {
    struct timeval t;
    t.tv_sec = msw - NTP_EPOCH_OFFSET;
    t.tv_usec = (uint32_t)((((double)lsw) * 1000000.0) / ((uint32_t)(~0)));
    return t;
}

uint32_t RTCP::rtcp_dlsr() {
    struct timeval now;
    double delay;

#ifdef _WIN32
    mingw_gettimeofday(&now,NULL);
#else
    gettimeofday(&now, NULL);
#endif
    delay= (now.tv_sec - this->senderReport.tvSec) +
           ((now.tv_usec - this->senderReport.tvUSec) * 1e-6);
//    delay= (delay * 65536);
    //std::cout << "delay: " << delay << std::endl;
    return (uint32_t) delay;
}

/* create a receiver report package */
int RTCP::receiver_report() {
    return 0;
}

