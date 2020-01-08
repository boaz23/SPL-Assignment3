
#ifndef STOMPCLIENT_BOOKLIBRARYUSER_H
#define STOMPCLIENT_BOOKLIBRARYUSER_H

#include <string>
#include <unordered_map>
#include "UserBooks.h"
#include "../stomp/StompConnectionHandler.h"

class BookLibraryUser {
private:
    std::string username;
    std::string password;

    StompConnectionHandler &_connection;
    StompMessageEncoderDecoder &_encdec;

    UserBooks books;
    std::unordered_map<int, Frame&> receipts;
    std::unordered_map<std::string, std::string> pendingBorrows;
    std::unordered_map<std::string, std::string> sucessfulBorrows;
public:
    BookLibraryUser(std::string username, std::string password, StompConnectionHandler &connection, StompMessageEncoderDecoder &encdec);

    bool connect();
    void run(); // TODO: start thread and input stream loop
};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
