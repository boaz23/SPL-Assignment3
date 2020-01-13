
#include "../../include/stomp/StompConnectionHandler.h"
#include <utility>

StompConnectionHandler::StompConnectionHandler(std::string host, short port, StompMessageEncoderDecoder &encdec) :
ConnectionHandler(std::move(host), port), _encdec(encdec) {}

bool StompConnectionHandler::sendFrame(Frame &frame) {
    std::unique_ptr<std::string> str(_encdec.encode(frame));
    return sendBytes(str->c_str(), str->length());
}
