
#ifndef STOMPCLIENT_STOMPCONNECTIONHANDLER_H
#define STOMPCLIENT_STOMPCONNECTIONHANDLER_H

#include <string>
#include "../api/ConnectionHandler.h"
#include "StompMessageEncoderDecoder.h"

#define DEBUG_PRINT_FRAMES

class StompConnectionHandler : public ConnectionHandler {
private:
    StompMessageEncoderDecoder &_encdec;

public:
    StompConnectionHandler(std::string host, short port, StompMessageEncoderDecoder &encdec);

    bool readFrame(std::unique_ptr<Frame> &frame);
    bool sendFrame(const Frame &frame);

#ifdef DEBUG_PRINT_FRAMES
    std::unique_ptr<std::string> encode(const Frame &frame);
#endif
};


#endif //STOMPCLIENT_STOMPCONNECTIONHANDLER_H
