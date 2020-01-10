
#include "../../include/bookLibrary/BookLibraryUser.h"

#include <utility>
#include "../../include/stomp/StompVersion.h"
#include "../../include/Util.h"

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

void BookLibraryUser::run() {
    std::unique_ptr<Frame> frame = nullptr;

    //TODO: check how to stop the thread, maybe use inperept
    //TODO: refactor
    while(true){
        if (!readFrame(frame)) {
            break;
        }

        if(frame->messageType() == "RECEIPT"){
            std::string receipt = frame->receiptId();
            if(receipts.count(receipt) > 0){
                Frame receiptFrame = receipts.at(receipt);

                if(receiptFrame.messageType() == "SUBSCRIBE"){
                    std::string message = "Joined club " + receiptFrame.headers().at("destination");
                    _printer.println(message);
                } else if(receiptFrame.messageType() == "UNSUBSCRIBE"){
                    std::string message = "E club " + receiptFrame.headers().at("destination");
                    _printer.println(message);
                } else if(receiptFrame.messageType() == "DISCONNECT"){
                    break;
                }

                receipts.erase(receipt);
                delete(&receiptFrame);
            }
        }
        else if(frame->messageType() == "MESSAGE"){
            std::string dest = frame->headers().at("destination");
            std::string body = frame->headers().at("body");

            printMessage(dest, body);

            std::vector<std::string> message = Util::split(body, " ");
            //TODO: check if body is empty

            if(body.find("wish to borrow") != std::string::npos){
                std::string name = message[0];
                std::string bookName = message[4];

                if(name != _username){
                    if(books.hasBook(bookName)){
                        sendHasBookFrame(dest, bookName);
                    }
                }
            } else if(message.size() == 3 && message[1] == "has"){
                std::string userOfTheBook = message[0];
                std::string bookName = message[2];
                if(books.wantToBorrow(bookName)){
                    sendTakingBookFrom(dest, bookName, userOfTheBook);
                    books.borrowBook(dest, bookName, userOfTheBook);
                }
            } else if(message.size() == 4 && message[0] == "Returning"){
                std::string bookName = message[1];
                std::string userOfTheBook = message[3];
                if(userOfTheBook == _username && books.isBorrowed(dest, bookName)){
                    books.returnBorrowedBook(dest, bookName);
                }
            } else if(message.size() == 2 && message[0] == "book" && message[1] == "status"){
                sendBooksStatus(dest);
            }
        }

    }
}

void BookLibraryUser::sendBooksStatus(const std::string &dest) {
    BookCollection bookCollection = books.bookCollection(dest);

    std::string bodyMessage = _username + ":";
    for(const Book book : bookCollection){
        bodyMessage.append(book.bookName());
        bodyMessage.append(",");
    }

    bodyMessage.resize(bodyMessage.size()-1);
    sendSendFrame(dest, bodyMessage);
}

void BookLibraryUser::sendHasBookFrame(const std::string &topic, const std::string &bookName) {
    std::string body = _username + " has " + bookName;
    sendSendFrame(topic ,body);
}

void BookLibraryUser::sendTakingBookFrom(const std::string &topic, const std::string &bookName, const std::string &from) {
    std::string body = "Taking " + bookName + " from " + from;
    sendSendFrame(topic ,body);
}

void BookLibraryUser::printMessage(const std::string &topic, const std::string &body) {
    std::string message = topic + ":" + body;
    _printer.println(message);
}

void BookLibraryUser::sendSendFrame(const std::string &topic, const std::string &body) {
    SendFrame frame = SendFrame(topic ,body);
    _connection->sendFrame(frame);
}
