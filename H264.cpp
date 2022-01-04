//
// Created by alpine on 2021/06/24.
//

#include <cstring>
#include <fstream>   // ifstream, ofstream
#include <vector>
#include "include/H264.h"
#include "util.cpp"

void H264::fromSdp(std::string sprop) {
    size_t sps_len;
    size_t pps_len;
    /* SPS */
//    char *sep = strchr(sprop.c_str(), ',');
//    int p_size = (sep - sprop.c_str());
//    char *sps_base64 = (char *)malloc(p_size + 1);
//    memset(sps_base64, '\0', p_size + 1);
//    memcpy(sps_base64, sps_base64, p_size);

    std::basic_string<char>::size_type sep = sprop.find(',');
    std::string sps_base64 = sprop.substr(0, sep);
    this->sps = this->base64Decode(sps_base64);

    /* PPS */
    std::string pps_base64 = sps_base64.substr(sep);
    this->pps = this->base64Decode(pps_base64);
}

void H264::decode(std::vector<unsigned char> payload) {
    int i = 0;
    this->decodeHeader(payload, i);
}

void H264::decodeHeader(std::vector<unsigned char> payload, int i) {
    this->header.forbiddenZero = payload[i] >> 7;
    this->header.refIdc = (payload[i] & 0x60) >> 5;
    this->header.unitType = (payload[i] & 0x1f);
    this->printHeader();

    std::vector<unsigned char> test;

    i++;
    switch ((UnitType) this->header.unitType) {
        case UnitType::sliceDataA...UnitType::sliceExtensionDepth:
            this->writeNAL();
            this->write(payload.data());
            break;
        case UnitType::FUA:
            uint8_t startBit;
            uint8_t endBit;
            uint8_t reservedBit;
            uint8_t unitType;
            uint8_t key;

            startBit    = payload[i] >> 7;
            endBit      = (payload[i] & 0x40) >> 6;
            reservedBit = (payload[i] & 0x20) >> 5;
            unitType    = (payload[i] & 0x1F);
            key         = this->header.refIdc << 5 | unitType;

            if ( unitType == 5 && debug) {
                std::cout << format("   Unit Type    : %u", unitType) << std::endl;
                std::cout << format("   %u", payload[i] >> 7) << std::flush;
                std::cout << format("%u", (payload[i] & 0x40) >> 6) << std::flush;
                std::cout << format("%u", (payload[i] & 0x20) >> 5) << std::flush;
                std::cout << format("%u", (payload[i] & 0x10) >> 4) << std::flush;
                std::cout << format("%u", (payload[i] & 0x08) >> 3) << std::flush;
                std::cout << format("%u", (payload[i] & 0x04) >> 2) << std::flush;
                std::cout << format("%u", (payload[i] & 0x02) >> 1) << std::flush;
                std::cout << format("%u", (payload[i] & 0x01) >> 0) << std::endl;
            }

            if (startBit) {
                this->writeNAL();
                this->write(&key);
            }

            test = {payload.begin() + 2, payload.end()};
            this->write(test.data());

            if (debug) {
                std::cout << ">> FU Header" << std::endl;
                std::cout << format("   Start Bit    : %u", startBit) << std::endl;
                std::cout << format("   End Bit      : %u", endBit) << std::endl;
                std::cout << format("   Reserved Bit : %u", reservedBit) << std::endl;
                std::cout << format("   Unit Type    : %u", unitType) << std::endl;
            }

            break;
        case UnitType::STAPA:
            uint8_t *q;
            uint16_t nalu_size;

            q = payload.data() + 1;
            int nidx = 0;

            while (nidx < payload.size() - 1) {
                /* write NAL header */
                this->writeNAL();

                /* get NALU size */
                nalu_size = (q[nidx] << 8) | (q[nidx + 1]);
                nidx += 2;

                /* write NALU size */
                this->write(&nalu_size);

                if (nalu_size == 0) {
                    nidx++;
                    continue;
                }

                /* write NALU data */
                this->write(q + nidx);
                nidx += nalu_size;
            }
            break;
    }
}

void H264::printHeader() const {
    /*
     * NAL, first byte header
     *
     *   +---------------+
     *   |0|1|2|3|4|5|6|7|
     *   +-+-+-+-+-+-+-+-+
     *   |F|NRI|  Type   |
     *   +---------------+
     */
    if (this->debug) {
        std::cout << ">> NAL Header" << std::endl;
        std::cout << format("   Forbidden Zero : %u", this->header.forbiddenZero) << std::endl;
        std::cout << format("   NAL ref idc    : %u", this->header.refIdc) << std::endl;
        std::cout << format("   Unit Type      : %u", this->header.unitType) << std::endl;
    }
}

void H264::openWriteFile() {
    this->writingFile.open(this->filename, std::ios::out);
//    std::cout << format("File: %i", this->writingFile.is_open()) << std::endl;
}

void H264::writeHeader() {
    uint8_t nal_header[4] = {0x00, 0x00, 0x00, 0x01};
    this->writingFile.write(reinterpret_cast<const char *>(nal_header), sizeof(nal_header));
    this->writingFile.write(this->sps.c_str(), this->sps.size());

    this->writingFile.write(reinterpret_cast<const char *>(nal_header), sizeof(nal_header));
    this->writingFile.write(this->pps.c_str(), this->pps.size());
}

void H264::write(const void *buf) {
    this->writingFile.write(reinterpret_cast<const char *>(buf), sizeof(buf));
}

void H264::writeNAL() {
    uint8_t nal_header[4] = {0x00, 0x00, 0x00, 0x01};
    this->writingFile.write(reinterpret_cast<const char *>(nal_header), sizeof(nal_header));
}

/*
    BASE64デコード
*/
std::string H264::base64Decode(std::string strData) {
    /*
        変換テーブル
    */
    const static unsigned char baTable[ 256 ] = {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f
            , 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e
            , 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28
            , 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };

    std::string strRet;
    std::string::iterator ite    = strData.begin();
    std::string::iterator iteEnd = strData.end();

    while( ite != iteEnd ) {

        unsigned long dwValue = 0;
        int nBitCount = 0;

        // 4バイト単位で処理する
        int nCount = 4;
        while( nCount-- ) {

            // 1バイト取得
            dwValue <<= 6;
            if ( ite != iteEnd ) {
                unsigned char bChar = baTable[ *ite++ ];
                if ( 64 > bChar ) {
                    dwValue |= bChar;
                    nBitCount += 6;
                }
                else if ( 0xfe == bChar ) {

                    // 改行コードは完全に無視する
                    nCount++;
                    dwValue >>= 6;
                }
            }
        }

        auto* baTmp = (unsigned char*)&dwValue;

        // 並べ替え
        unsigned char baWrite[ 3 ] = { baTmp[ 2 ], baTmp[ 1 ], baTmp[ 0 ] };

        // 最大３バイトの書き込み
        int nByte = nBitCount / 8;
        if ( 0 < nByte ) {
            strRet.append( (char*)baWrite, nByte );
        }
    }

    // BASE64デコード結果を返す
    return strRet;
}

H264::H264(const H264 &decode) {
    this->openWriteFile();
}

H264::H264(int is_dump) : isDump(is_dump) {
    this->openWriteFile();
}
