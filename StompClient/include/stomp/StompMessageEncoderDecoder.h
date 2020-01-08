
#ifndef STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
#define STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H

#include "../api/MessageEncoderDecoder.h"
#include "Frame.h"
#include <vector>

class StompMessageEncoderDecoder : public MessageEncoderDecoder<Frame, Frame> {
private:
    std::vector byteVector;

    void decodeHeaders(const Frame &message, const std::string *data) const;
    void decodeMessage(const Frame &message, std::string *data) const;
    void decodeBody(const Frame &message, const std::string *data) const;
    void encodeMessage(Frame *frame, int &index) const;
    void encodeKey(int &index, std::string &key) const;
    void encodeValue(int &index, std::string &value) const;

    Frame* createFrame();
public:

    StompMessageEncoderDecoder();
    virtual Frame* decodeNextByte(byte nextByte);
    virtual Array<byte> encode( Frame &message);

};


#endif //STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
