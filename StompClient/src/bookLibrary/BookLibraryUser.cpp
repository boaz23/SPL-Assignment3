
#include "../../include/bookLibrary/BookLibraryUser.h"
#include "../../include/stomp/StompVersion.h"

BookLibraryUser::BookLibraryUser(std::string username, std::string password, StompConnectionHandler &connection,
                                 StompMessageEncoderDecoder &encdec) : username(username), password(password), _connection(connection), _encdec(encdec),
                                 books(), receipts(), pendingBorrows(), sucessfulBorrows() {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"
bool BookLibraryUser::connect() {
    ConnectFrame connectFrame(ACCEPT_VERSION, _connection.host(), username, password);
    if (!_connection.sendFrame(connectFrame)) {
        return false;
    }

    std::unique_ptr<Frame> frame = nullptr;
    if (!readFrame(frame)) {
        return false;
    }

    ConnectedFrame connectedFrame(std::move(*frame));
    if (!connectedFrame.isValid() || connectedFrame.version() != ACCEPT_VERSION) {
        return false;
    }

    return true;
}
#pragma clang diagnostic pop

bool BookLibraryUser::readFrame(std::unique_ptr<Frame> &frame) {
    bool ret;
    byte  b = 0;
    while ((ret = !_connection.getBytes(&b, 1))) {
        Frame* frameTmp = _encdec.decodeNextByte(b);
        if (frameTmp) {
            frame.reset(frameTmp);
            break;
        }
    }

    return ret;
}