
#include "../../include/stomp/StompConnectionHandler.h"
#include <utility>

StompConnectionHandler::StompConnectionHandler(std::string host, short port, StompMessageEncoderDecoder &encdec) :
ConnectionHandler(std::move(host), port), _encdec(encdec) {}

bool StompConnectionHandler::sendFrame(Frame &frame) {
    Array<byte> bytes = _encdec.encode(frame);
    return sendBytes(bytes.array, bytes.length);
}
