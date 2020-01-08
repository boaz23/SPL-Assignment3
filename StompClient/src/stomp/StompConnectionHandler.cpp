
#include "../../include/stomp/StompConnectionHandler.h"
#include <memory>

bool StompConnectionHandler::sendFrame(const Frame &frame) {
    Array<byte> bytes = encdec.encode(ConnectFrame());
    connectionHandler.sendBytes(bytes.array, bytes.length);
}
