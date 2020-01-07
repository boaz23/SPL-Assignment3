
#ifndef STOMPCLIENT_STOMPCONNECTIONHANDLER_H
#define STOMPCLIENT_STOMPCONNECTIONHANDLER_H

#include <string>
#include "../api/ConnectionHandler.h"
#include "StompMessageEncoderDecoder.h"

class StompConnectionHandler : public ConnectionHandler {
private:
    StompMessageEncoderDecoder &_enddec;

public:
    StompConnectionHandler(std::string host, short port, Printer &printer, StompMessageEncoderDecoder &enddec);
    bool sendFrame(const Frame &frame);
};


#endif //STOMPCLIENT_STOMPCONNECTIONHANDLER_H
