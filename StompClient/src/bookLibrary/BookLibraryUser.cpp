
#include "../../include/bookLibrary/BookLibraryUser.h"

#include <utility>
#include "../../include/stomp/StompVersion.h"

BookLibraryUser::BookLibraryUser(
    std::string username, std::string password,
    Printer &printer
) : _username(std::move(username)), _password(std::move(password)),
    _connection(nullptr), _encdec(nullptr), _printer(printer),
    books(), receipts(), pendingBorrows(), successfulBorrows() {}

void BookLibraryUser::setConnection(StompConnectionHandler *connection) {
    _connection = connection;
}

void BookLibraryUser::setEncoderDecoder(StompMessageEncoderDecoder *encdec) {
    _encdec = encdec;
}

std::string BookLibraryUser::username() {
    return _username;
}

bool BookLibraryUser::connect(std::string &errorMsg) {
    ConnectFrame connectFrame(ACCEPT_VERSION, _connection->host(), _username, _password);
    if (!_connection->sendFrame(connectFrame)) {
        return false;
    }

    std::unique_ptr<Frame> frame = nullptr;
    if (!readFrame(frame)) {
        return false;
    }

    ConnectedFrame connectedFrame(std::move(*frame));
    if (connectedFrame.isValid()) {
        if (connectedFrame.version() != ACCEPT_VERSION) {
            return false;
        }
    }
    else {
        ErrorFrame errorFrame(std::move(connectedFrame));
        if (errorFrame.isValid()) {
            errorMsg = errorFrame.errorMessage();
        }

        return false;
    }

    return true;
}

bool BookLibraryUser::readFrame(std::unique_ptr<Frame> &frame) {
    bool ret;
    byte  b = 0;
    while ((ret = !_connection->getBytes(&b, 1))) {
        Frame* frameTmp = _encdec->decodeNextByte(b);
        if (frameTmp) {
            frame.reset(frameTmp);
            break;
        }
    }

    return ret;
}