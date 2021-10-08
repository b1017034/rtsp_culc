//
// Created by alpine on 2021/06/23.
//

#include <cstring>
#include <iostream>
#include <utility>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/param.h>
#include <netinet/in.h>

#include "RTSP.h"
#include "util.cpp"

RTSP::RTSP(int sock, std::string stream, bool is_dump) {
    this->sock = sock;
    this->stream = std::move(stream);
    this->h264 = new H264(is_dump);

    this->rtcp = new RTCP(sock);
    this->rtp = new RTP(*this->h264, this->rtcp);

}

RTSP::RTSP(int sock, bool is_dump) {
  this->serverSock = sock;
  this->h264 = new H264(is_dump);

  this->rtcp = new RTCP(sock);
  this->rtp = new RTP(*this->h264, this->rtcp);
}

RTSP::RTSP(bool is_dump) {
    this->h264 = new H264(is_dump);

    this->rtcp = new RTCP(sock);
    this->rtp = new RTP(*this->h264, this->rtcp);
}

int RTSP::cmdOptions() {
    std::string host = "127.0.0.1";
    unsigned int port = 8554;

    std::string error;
    std::string sendbuf;
    char buf[4096];
    int status;
    int n;

    //サーバーにデータを送信
    std::cout << "OPTIONS: command" << std::endl;
    std::cout << format(CMD_OPTIONS, host.c_str(), port, this->sequence)  << std::endl;
    sendbuf = format(CMD_OPTIONS, host.c_str(), port, this->sequence);
    n = send(this->sock, sendbuf.c_str(), sendbuf.size(), 0);
    if (n <= 0) std::cout << "send failed" << std::endl;

    // サーバからデータを受信
    std::cout << "OPTIONS: response" << std::endl;
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    if (n <= 0) {
        std::cout << "Error: Server did not respond properly, closing..." << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return EXIT_FAILURE;
    }
    std::cout << buf << std::endl;

    char* cerror = new char[error.size() + 1];
    std::char_traits<char>::copy(cerror, error.c_str(), error.size() + 1);
    status = RTSP::responseStatus(buf, &cerror);
    error = cerror;
    if (status == 200) {
        std::cout << format("OPTIONS: response status %i (%i bytes)", status, n) << std::endl;
    } else {
        std::cout << format("OPTIONS: response status %i: %s", status, error.c_str()) << std::endl;
    }

    RTSP::sequenceIncrease();

    return 0;
}

int RTSP::cmdDescribe() {
    std::string error;
    std::string sendbuf;
    char buf[4096];
    int status;
    int n;

    //サーバーにデータを送信
    std::cout << "DESCRIBE: command" << std::endl;
    sendbuf = format(CMD_DESCRIBE, this->stream.c_str() , this->sequence);
    std::cout << sendbuf  << std::endl;

    n = send(this->sock, sendbuf.c_str(), sendbuf.size(), 0);
    if (n <= 0) std::cout << "send failed" << std::endl;

    // サーバからデータを受信
    std::cout << "DESCRIBE: response" << std::endl;
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    if (n <= 0) {
        std::cout << "Error: Server did not respond properly, closing..." << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return 1;
    }
    std::cout << buf << std::endl;

    //レスポンス取得
    char* cerror = new char[error.size() + 1];
    std::char_traits<char>::copy(cerror, error.c_str(), error.size() + 1);
    status = RTSP::responseStatus(buf, &cerror);
    error = cerror;
    if (status == 200) {
        std::cout << format("DESCRIBE: response status %i (%i bytes)", status, n) << std::endl;
    } else {
        std::cout << format("DESCRIBE: response status %i: %s", status, error.c_str()) << std::endl;
    }

    RTSP::sequenceIncrease();

    /* set the DSP information */
    char *p = strstr(buf, "\r\n\r\n");
    if (!p) {
        return -1;
    }

    /* Create buffer for DSP */
    char *cdsp = (char*)malloc(n + 1);
    memset(cdsp, '\0', n + 1);
    strcpy(cdsp, p + 4);
    this->dsp = cdsp;

    /* sprop-parameter-sets key */
    char *dep_c = (char *)dsp.c_str();
    p = strstr(dep_c, RTP_SPROP);
//    p = reinterpret_cast<char *>(this->dsp.find(RTP_SPROP));
    if (!p) {
        return -1;
    }

    char *end = strchr(p, '\r');
    if (!end) {
        return -1;
    }

    int prop_size = (end - p) - sizeof(RTP_SPROP) + 1;
    this->sprop.assign(p + sizeof(RTP_SPROP) - 1, prop_size);

    return 0;
}

int RTSP::cmdSetup() {
    std::string error;
    std::string sendbuf;
    char buf[4096];
    int status;
    int n;

    //サーバーにデータを送信
    std::cout << "SETUP: command" << std::endl;
    sendbuf = format(CMD_SETUP, this->stream.c_str() , this->sequence);
    std::cout << sendbuf  << std::endl;

    n = send(this->sock, sendbuf.c_str(), sendbuf.size(), 0);
    if (n <= 0) std::cout << "send failed" << std::endl;

    // サーバからデータを受信
    std::cout << "SETUP: response" << std::endl;
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    if (n <= 0) {
        std::cout << "Error: Server did not respond properly, closing..." << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return EXIT_FAILURE;
    }
    std::cout << buf << std::endl;

    //レスポンス取得
    char* cerror = new char[error.size() + 1];
    std::char_traits<char>::copy(cerror, error.c_str(), error.size() + 1);
    status = RTSP::responseStatus(buf, &cerror);
    error = cerror;
    if (status == 200) {
        std::cout << format("SETUP: response status %i (%i bytes)", status, n) << std::endl;

        //get session id
        std::string buf_str = buf;
        std::basic_string<char>::size_type session_start = buf_str.find(SETUP_SESSION);
        std::string session_id = buf_str.substr(session_start + sizeof(SETUP_SESSION) - 1, 16);
        int sessionEnd = session_id.find(';');
        if (sessionEnd != session_id.npos) session_id = session_id.erase(sessionEnd);

        // Fill session data
        this->sessionInfo.packetization = 1; /* FIXME: project specific value */
        this->sessionInfo.sessionId = session_id;
        //if use no interleaved
//        this->SessionInfo.clientPortFrom =
//        this->SessionInfo.clientPortTo =
//        this->SessionInfo.serverPortFrom =
//        this->SessionInfo.sport_t =

    } else {
        std::cout << format("SETUP: response status %i: %s", status, error.c_str()) << std::endl;
        return -1;
    }

    RTSP::sequenceIncrease();

    return 0;
}

int RTSP::cmdPlay() {
    std::string error;
    std::string sendbuf;
    char buf[4096];
    int status;
    int n;

    //サーバーにデータを送信
    std::cout << "PLAY: command" << std::endl;
    sendbuf = format(CMD_PLAY, this->stream.c_str() , this->sequence, this->sessionInfo.sessionId.c_str());
    std::cout << sendbuf  << std::endl;

    n = send(this->sock, sendbuf.c_str(), sendbuf.size(), 0);
    if (n <= 0) std::cout << "send failed" << std::endl;

    // サーバからデータを受信
    std::cout << "PLAY: response" << std::endl;
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    if (n <= 0) {
        std::cout << "Error: Server did not respond properly, closing..." << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return -1;
    }
    std::cout << buf << std::endl;

    //レスポンス取得
    char* cerror = new char[error.size() + 1];
    std::char_traits<char>::copy(cerror, error.c_str(), error.size() + 1);
    status = RTSP::responseStatus(buf, &cerror);
    error = cerror;
    if (status == 200) {
        std::cout << format("PLAY: response status %i (%i bytes)", status, n) << std::endl;

    } else {
        std::cout << format("PLAY: response status %i: %s", status, error.c_str()) << std::endl;
        return -1;
    }

    RTSP::sequenceIncrease();
    return 0;
}

int RTSP::responseStatus(char *response, char **error) {
    int size = 256;
    int err_size;
    int offset = sizeof(RTSP_RESPONSE) - 1;
    char buf[8];
    char *sep;
    char *eol;
    *error = NULL;

    if (strncmp(response, RTSP_RESPONSE, offset) != 0) {
        *error = (char *)malloc(size);
        snprintf(*error, size, "Invalid RTSP response format");
        return -1;
    }

    sep = strchr(response + offset, ' ');
    if (!sep) {
        *error = (char *)malloc(size);
        snprintf(*error, size, "Invalid RTSP response format");
        return -1;
    }

    memset(buf, '\0', sizeof(buf));
    strncpy(buf, response + offset, sep - response - offset);

    eol = strchr(response, '\r');
    err_size = (eol - response) - offset - 1 - strlen(buf);
    *error = (char *)malloc(err_size + 1);
    strncpy(*error, response + offset + 1 + strlen(buf), err_size);

    return atoi(buf);
}

int RTSP::decode(std::vector<unsigned char> raw, int raw_length) {
  int raw_offset = 0;
  if (raw.size() > 4 && raw[0] == 'R' && raw[1] == 'T') {
    std::cout << std::string (raw.begin(), raw.end()) << std::endl;
  }
  if(this->tmpBuffer.size() > 0) {
//    printf("raw_length: %i", raw_length);
    tmpBuffer.insert(tmpBuffer.end(), raw.begin(), raw.begin() + raw_length);

    raw.clear();
    raw = { tmpBuffer.begin(), tmpBuffer.end() };
    tmpBuffer.clear();
    raw_length = raw.size();
//    printf("raw_length: %i", raw_length);
  }
  while ( raw_offset < raw_length) {
//    printf("offset %i interleaved: %i\n", raw_offset, raw[raw_offset] == 0x24);

    /* RTSP Interleaved header */
    if (raw[raw_offset] == 0x24) {

      /* RTSP header is 4 bytes, update the offset */
      raw_offset += this->decodeInterleavedFrame(raw, raw_offset, raw_length);

      if (raw_offset + this->interleavedFrame.rtpLength > raw_length) {
        raw_offset -= 4;
        printf("Pending: raw_offset = %i ; rtp_length = %i; raw_length = %i\n",
               raw_offset, this->interleavedFrame.rtpLength, raw_length);

        this->tmpBuffer = { raw.begin() + raw_offset, raw.begin() + raw_length };
        return raw_length - raw_offset;
      }

      /* If no payload exists, continue with next bytes */
      if (this->interleavedFrame.rtpLength == 0) {
        raw_offset -= 4;
        continue;
      }

      /* If RTP length is longer than payload, pending*/
      if (this->interleavedFrame.rtpLength > (raw_length - raw_offset)) {
        raw_offset -= 4;
        if (this->debug) printf("   ** Pending    : %u bytes\n",
                                this->interleavedFrame.rtpLength - (raw_length - raw_offset));
      }

      if (this->rtp->stats.rtpReceived % 20) {
//                    this->sendRTCPRR();
      }

      /* RTCP data */
      if (this->interleavedFrame.channel >= 1) {
        /* Decode RTCP packet(s) */
        this->rtcp->decode({raw.begin() + raw_offset, raw.begin() + raw_offset + this->interleavedFrame.rtpLength});
        this->rtp->stats.delaySncLastSr = this->rtcp->rtcp_dlsr();
        this->sendRTCPRR();
//                    if (rtcp_count >= 1 && RTCP[0].type == RTCP_SR) {
//                        send_rtcp = 1;
//                        //rtsp_rtcp_reports(fd);
//                    }
        std::cout << ">>RTCP Header" << std::endl;
        std::cout << format("       TimeStamp: %lu", this->rtcp->stats.lastRtpTimestamp) << std::endl;

        raw_offset += this->interleavedFrame.rtpLength;
        continue;
      }
      /*
       * Channel 0
       * ---------
       * If the channel is zero, the payload should contain RTP data,
       * we need to identify the RTP header fields so later we can
       * proceed to extract the H264 information.
       */
      int offset;
      offset = this->rtp->decode({raw.begin() + raw_offset, raw.begin() + raw_offset + this->interleavedFrame.rtpLength});
      raw_offset += this->interleavedFrame.rtpLength;
      continue;
    }
    raw_offset++;
  }
  return 0;
}

int RTSP::decodeInterleavedFrame(std::vector<unsigned char> buffer, int buffer_offset, int buffer_length) {
    if(buffer.size() < 4) return 0;

    this->interleavedFrame.channel = buffer[buffer_offset + 1];
    this->interleavedFrame.rtpLength  = (buffer[buffer_offset + 2] << 8 | buffer[buffer_offset + 3]);

    if (this->debug) {
        std::cout << format( ">> RTSP Header") << std::endl;
        std::cout << format( "RTSP Interleaved (offset = %i/%i)", buffer_offset, buffer_length) << std::endl;
        std::cout << format( "Magic         : 0x24") << std::endl;
        std::cout << format( "Channel       : %i", this->interleavedFrame.channel) << std::endl;
        std::cout << format( "Payload Length: %i", this->interleavedFrame.rtpLength) << std::endl;
    }

    return 4;
}

void RTSP::sendRTCPRR() {
    this->rtcp->sendReceiverReport();
}

int RTSP::rtspLoop() {
//    this->cmdOptions();
//    this->cmdDescribe();
//    this->cmdSetup();
//    this->cmdPlay();
//
//    this->h264->fromSdp(this->sprop);
//    this->h264->openWriteFile();
//    this->h264->writeHeader();

    int n;
    int max_buf_size = 10000;
//    unsigned char raw[max_buf_size];
    std::vector<unsigned char> raw(max_buf_size);
    std::vector<unsigned char> raw_tmp(max_buf_size);
    while (true) {
        int raw_offset = 0;
        int raw_length = 0;

        read_pending:
        /* is buffer full ? */
        if (raw_length >= max_buf_size) {
            std::cout << "RESETTING" << std::endl;
            if ( raw_offset < raw_length - 1) {
                int bytes = raw_length - raw_offset;
                copy(  raw.begin() + raw_offset, raw.begin() + raw_offset + bytes, raw.begin() );
//                raw = { raw.begin() + raw_offset, raw.begin() + raw_offset + bytes };
                raw_length = bytes;
                raw_offset = 0;
                if (raw[raw_offset] != 0x24) {
                    std::cout << "MASTER CORRUPTION" << std::endl;
                } else {
                    std::cout << "Copied Size: " << raw_tmp.size() << std::endl;
                }
            }
            else {
                raw_length = 0;
            }
        }

        // サーバからデータを受信
        if (this->debug) std::cout << format("RECV: max %i bytes", max_buf_size - raw_length) << std::endl;
        n = recv(sock, reinterpret_cast<char *>(&raw[raw_length]), max_buf_size - raw_length, 0);
        if (this->debug ) std::cout << format("READ: %i (up to %i)", n, max_buf_size - raw_length) << std::endl;
        if (n <= 0) {
            continue;
//            return -1;
        }

        raw_length += n;

        while ( raw_offset < raw_length) {
            /* RTSP Interleaved header */
            if (raw[raw_offset] == 0x24) {

                /* RTSP header is 4 bytes, update the offset */
                raw_offset += this->decodeInterleavedFrame(raw, raw_offset, raw_length);

                if (raw_length > max_buf_size) {
                    printf("Error exception: raw_length = %i ; max_buf_size = %i\n",
                           raw_length, max_buf_size);
                    return -1;
                }

                /* If no payload exists, continue with next bytes */
                if (this->interleavedFrame.rtpLength == 0) {
                    raw_offset -= 4;
                    goto read_pending;
                    continue;
                }

                /* If RTP length is longer than payload, pending*/
                if (this->interleavedFrame.rtpLength > (raw_length - raw_offset)) {
                    raw_offset -= 4;
                    if (this->debug) printf("   ** Pending    : %u bytes\n",
                                            this->interleavedFrame.rtpLength - (raw_length - raw_offset));

//                    if (send_rtcp == 1){
//                        rtsp_rtcp_reports(fd);
//                        send_rtcp = 0;
//                    }
                    goto read_pending;
                }

                if (this->rtp->stats.rtpReceived % 20) {
//                    this->sendRTCPRR();
                }

                /* RTCP data */
                if (this->interleavedFrame.channel >= 1) {
                    /* Decode RTCP packet(s) */
                    this->rtcp->decode({raw.begin() + raw_offset, raw.begin() + raw_offset + this->interleavedFrame.rtpLength});
                    this->rtp->stats.delaySncLastSr = this->rtcp->rtcp_dlsr();
//                    this->sendRTCPRR();
//                    if (rtcp_count >= 1 && RTCP[0].type == RTCP_SR) {
//                        send_rtcp = 1;
//                        //rtsp_rtcp_reports(fd);
//                    }
                    std::cout << ">>RTCP Header" << std::endl;
                    std::cout << format("       TimeStamp: %lu", this->rtcp->stats.lastRtpTimestamp) << std::endl;

                    raw_offset += this->interleavedFrame.rtpLength;
                    continue;
                }

                if (this->interleavedFrame.rtpLength == 0) {
                    continue;
                }

                /*
                 * Channel 0
                 * ---------
                 * If the channel is zero, the payload should contain RTP data,
                 * we need to identify the RTP header fields so later we can
                 * proceed to extract the H264 information.
                 */
                int offset;
                offset = this->rtp->decode({raw.begin() + raw_offset, raw.begin() + raw_offset + this->interleavedFrame.rtpLength});
                raw_offset += this->interleavedFrame.rtpLength;
                continue;
            }
            raw_offset++;
        }
    }
    return 0;
}

void RTSP::sequenceIncrease(){
    this->sequence ++;
}

