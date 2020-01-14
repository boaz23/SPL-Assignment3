
#include "../../include/stomp/StompConnectionHandler.h"
#include <utility>

StompConnectionHandler::StompConnectionHandler(StompMessageEncoderDecoder &encdec) :
    ConnectionHandler(), _encdec(encdec) {}


bool StompConnectionHandler::readFrame(std::unique_ptr<Frame> &frame) {
    bool ret;
    byte  b = 0;
    while ((ret = getBytes(&b, 1))) {
        frame = _encdec.decodeNextByte(b);
        if (frame) {
            break;
        }
    }

    return ret;
}

bool StompConnectionHandler::sendFrame(const Frame &frame) {
    std::unique_ptr<std::string> str(_encdec.encode(frame));
    return sendBytes(str->c_str(), str->length());
}

#ifdef DEBUG_PRINT_FRAMES
std::unique_ptr<std::string> StompConnectionHandler::encode(const Frame &frame) {
    return _encdec.encode(frame);
}
#endif
