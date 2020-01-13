
#ifndef STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
#define STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H

#include "Frame.h"
#include <vector>
#include <memory>

using byte = char;

class StompMessageEncoderDecoder {
private:
    std::vector<byte> byteVector;

    void encodeHeaders(const Frame &message, std::string &data) const;
    void encodeMessage(const Frame &message, std::string &data) const;
    void encodeBody(const Frame &message, std::string &data) const;
    void getLine(std::string &line, size_t &index) const;
    void decodeKey(size_t &index, std::string &key) const;
    void decodeValue(size_t &index, std::string &value) const;
    void decodeBody(Frame *frame, size_t &index) const;

    std::unique_ptr<Frame> buildFrame();
    static Frame* createFrameInstance(const std::string &line);

public:
    StompMessageEncoderDecoder();
    std::unique_ptr<Frame> decodeNextByte(byte nextByte);
    std::unique_ptr<std::string> encode(const Frame &message);
};


#endif //STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
