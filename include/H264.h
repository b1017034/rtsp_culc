//
// Created by alpine on 2021/06/24.
//

#ifndef CALC_RTSP_H264_H
#define CALC_RTSP_H264_H

#include <string>
#include <iostream>
#include <fstream>

static const unsigned char base64_table[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
class H264 {
    enum class UnitType {
        //Single NAL
        sliceNonIDR = 1,
        sliceDataA,
        sliceDataB,
        sliceDataC,
        sliceIDR,
        SEI,
        SPS,
        PPS,
        AUD,
        endSequence,
        endStream,
        filterData,
        extension,
        prefix,
        subset,
        //Reserved 16-18
        sliceAuxPic = 19,
        sliceExtension,
        sliceExtensionDepth,
        //Reserved 22-23
        //Aggregation Packets
        STAPA = 24,
        STAPB,
        MTAP16,
        MTAP24,
        //Fragmentation Units
        FUA,
        FUB,
        //reserved 30-31
    };

    struct Header {
        uint8_t forbiddenZero;
        uint8_t refIdc;
        uint8_t unitType;
    };

public:
    H264(const H264& decode);
    explicit H264(int is_dump);

    void fromSdp(std::string sprop);
    void decode(std::vector<unsigned char> payload);
    void openWriteFile();
    void write(const void *buf);
    void writeNAL();
    void writeHeader();

    std::string base64Decode(std::string strData);

private:
    //use file dump
    int isDump = true;
    std::ofstream writingFile;
    std::string sps;
    std::string pps;
    const std::string filename = "asahi.h264";

    //print debug
    bool debug = false;

    //model
    Header header{};

    //private functions

    void decodeHeader(std::vector<unsigned char> payload, int i);
    void printHeader() const;
};
#endif //CALC_RTSP_H264_H