
#ifndef STOMPCLIENT_BOOKLIBRARYUSER_H
#define STOMPCLIENT_BOOKLIBRARYUSER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "UserBooks.h"
#include "../stomp/StompConnectionHandler.h"

class BookLibraryUser {
private:
    std::string _username;
    std::string _password;

    StompConnectionHandler *_connection;
    StompMessageEncoderDecoder *_encdec;
    Printer &_printer;

    UserBooks books;
    std::unordered_map<int, Frame> receipts;
    std::unordered_map<std::string, std::string> pendingBorrows;
    std::unordered_map<std::string, std::string> successfulBorrows;
public:
    BookLibraryUser(std::string username, std::string password, Printer &printer);

    std::string username();

    bool connect(std::string &errorMsg);
    void run(); // TODO: start thread and input stream loop

    void addReceipt(const Frame &frame);
    void removeReceipt(const std::string &receiptId);

    void setConnection(StompConnectionHandler *connection);
    void setEncoderDecoder(StompMessageEncoderDecoder *encdec);
private:
    bool readFrame(std::unique_ptr<Frame> &frame);
};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
