
#ifndef STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
#define STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H

#include "../api/MessageEncoderDecoder.h"
#include "Frame.h"
#include <vector>

class StompMessageEncoderDecoder : public MessageEncoderDecoder<Frame, Frame> {
private:
    std::vector<byte> byteVector;

    void encodeHeaders(const Frame &message, std::string &data) const;
    void encodeMessage(const Frame &message, std::string &data) const;
    void encodeBody(const Frame &message, std::string &data) const;
    void decodeMessage(std::string &line, int &index) const;
    void decodeKey(int &index, std::string &key) const;
    void decodeValue(int &index, std::string &value) const;
    void decodeBody(Frame *frame, int &index) const;

    Frame* buildFrame();
    Frame* createFrameInstance(const std::string &line);

public:
    StompMessageEncoderDecoder();
    virtual Frame* decodeNextByte(byte nextByte);
    virtual Array<byte> encode(const Frame &message);
};


#endif //STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
