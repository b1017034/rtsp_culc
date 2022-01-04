//
// Created by alpine on 2021/06/24.
//

#ifndef CALC_RTSP_RTCP_H
#define CALC_RTSP_RTCP_H

#ifdef _WIN_32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstdint>
#include <vector>

const uint32_t NTP_EPOCH_OFFSET = static_cast<uint32_t>(2208988800ul);

class RTCP {
public:
    enum class PacketType {
        SR = 200,  //Sender Report
        RR,        //Receiver Report
        SDES,      //Source Description
        BYE,       //Goodbye
        APP,       //Application Defined
        FIR = 192, //Full INTRA-Frame request
        NACK,      //Negative acknowledgement
    };

    /* stats */
    struct Stats {
        uint32_t delaySncLastSR;
        timeval  lastSRTimestamp;
        uint32_t lastRtpTimestamp;
    };

    /* packet header */
    struct Header {
        std::uint8_t  version; //version number
        uint8_t  padding;      //padding
        uint8_t  item_count;   //item count
        PacketType  type;      //packet type
        uint16_t length;       //length
    };

    struct SenderReport {
        uint32_t sdSsrc;     //SSRC of sender
        uint32_t tsMsw;      //NTP timestamp, most significant word
        uint32_t tsLsw;      //NTP timestamp, least significant word
        uint32_t tsRtp;      //RTP timestamp
        uint32_t sdPktCount; //sender's packet count
        uint32_t sdOctCount; //sender's octet count
        timeval  timestamp;
        timeval  receiveTime;
        uint32_t tvSec;      //TimeValue Second
        uint32_t tvUSec;     //TimeValue Micro Second
    };

    enum class SDESType {
        BYE = 0, //Goodbye
        CNAME,   //Canonical End-Point Identifier
        NAME,    //User Name
        EMAIL,   //Electronic Mail Address
        PHONE,   //Phone Number
        LOC,     //Geographic User Location
        TOOL,    //Application or Tool Name
        NOTE,    //Notice/Status
        PRIV,    //Private Extensions
    };

    struct SourceDescription {
        uint32_t identifier;
        SDESType  type;
        uint8_t  length;
        uint16_t text;
    };

    struct ReceiverReport {
        uint32_t sdSsrc;  //SSRC of first source
        uint8_t  fraLos;  //fraction lost
        uint32_t cumPktLost; //cumulative number of packets lost
        uint32_t highSeq; //extended highest sequence number received
        uint32_t jitter;   //interarrival jitter
        uint32_t lsr;      //last SR (LSR)
        uint32_t dlsr;     //delay since last SR (DLSR)
    };
    Header header{};
    Stats stats{};
    SenderReport senderReport{};
    SourceDescription sourceDescription{};
    ReceiverReport receiverReport{};

    explicit RTCP(int socket): socket(socket) {}

    int receiver_report();
    void decode(std::vector<unsigned char> payload);
    int sendReceiverReport();
    uint32_t rtcp_dlsr();
private:
    int socket;

    bool debug = false;

    int decodeHeader(std::vector<unsigned char> payload, int i);
    int decodeSR(std::vector<unsigned char> payload, int i);
    int decodeSDES(std::vector<unsigned char> payload, int i);

    void printHeader() const;
    void printSR() const;
    void printSDES();

    timeval ntpToTimeval(uint32_t msw, uint32_t lsw);

    uint32_t culcDelayLastSR();
};


#endif //CALC_RTSP_RTCP_H
