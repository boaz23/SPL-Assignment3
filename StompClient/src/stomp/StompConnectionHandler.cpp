
#include "../../include/stomp/StompConnectionHandler.h"
#include <memory>

bool StompConnectionHandler::sendFrame(Frame &frame) {
    Array<byte> bytes = _encdec.encode(frame);
    sendBytes(bytes.array, bytes.length);
}
