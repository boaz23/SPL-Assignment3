
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

    /**
     * Decode byte byte until a Frame is completed
     * @param nextByte next byte to decode
     * @return Frame that was assembled by the previous bytes
     */
    std::unique_ptr<Frame> decodeNextByte(byte nextByte);

    /**
     * Encode the @param message To string
     * Take a Frame object and assemble a string from it.
     * @param message
     * @return
     */
    std::unique_ptr<std::string> encode(const Frame &message);
};


#endif //STOMPCLIENT_STOMPMESSAGEENCODERDECODER_H
