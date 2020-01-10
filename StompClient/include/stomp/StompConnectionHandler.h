
#ifndef STOMPCLIENT_STOMPCONNECTIONHANDLER_H
#define STOMPCLIENT_STOMPCONNECTIONHANDLER_H

#include <string>
#include "../api/ConnectionHandler.h"
#include "StompMessageEncoderDecoder.h"

class StompConnectionHandler : public ConnectionHandler {
private:
    StompMessageEncoderDecoder &_encdec;

public:
    StompConnectionHandler(std::string host, short port, Printer &printer, StompMessageEncoderDecoder &encdec);
    bool sendFrame(Frame &frame);
};


#endif //STOMPCLIENT_STOMPCONNECTIONHANDLER_H
