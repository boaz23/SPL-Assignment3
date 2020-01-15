
#ifndef STOMPCLIENT_STOMPCONNECTIONHANDLER_H
#define STOMPCLIENT_STOMPCONNECTIONHANDLER_H

#include <string>
#include "../api/ConnectionHandler.h"
#include "StompMessageEncoderDecoder.h"

//#define DEBUG_PRINT_FRAMES

class StompConnectionHandler : public ConnectionHandler {
private:
    StompMessageEncoderDecoder &_encdec;

public:
    StompConnectionHandler(StompMessageEncoderDecoder &encdec);

    /**
     * Read the data from the socket until a full frame is received
     * Insert the data to @param frame
     * @param frame The frame to fill up
     * @return True if valid frame is received, false if there was an error in the connection
     */
    bool readFrame(std::unique_ptr<Frame> &frame);

    /**
     * Write the @param frame to the socket connection
     * @param frame The frame to send
     * @return True if the send was completed, false if there was an error in the connection
     */
    bool sendFrame(const Frame &frame);

#ifdef DEBUG_PRINT_FRAMES
    std::unique_ptr<std::string> encode(const Frame &frame);
#endif
};


#endif //STOMPCLIENT_STOMPCONNECTIONHANDLER_H
