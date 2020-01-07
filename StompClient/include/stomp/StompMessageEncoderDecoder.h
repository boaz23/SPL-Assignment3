
#ifndef STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
#define STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H

#include "../api/MessageEncoderDecoder.h"
#include "Frame.h"

class StompMessageEncoderDecoder : public MessageEncoderDecoder<Frame, Frame> {
    // TODO: implement
    virtual Frame* decodeNextByte(byte nextByte);
    virtual Array<byte> encode(const Frame &message);
};


#endif //STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
