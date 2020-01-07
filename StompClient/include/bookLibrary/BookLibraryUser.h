
#ifndef STOMPCLIENT_BOOKLIBRARYUSER_H
#define STOMPCLIENT_BOOKLIBRARYUSER_H

#include <string>
#include "UserBooks.h"
#include "../stomp/StompConnectionHandler.h"

class BookLibraryUser {
private:
    UserBooks books;
    std::string username;

    StompConnectionHandler &_connection;
    StompMessageEncoderDecoder &_encdec;
public:
    BookLibraryUser(std::string username, StompConnectionHandler &connection, StompMessageEncoderDecoder &encdec);

    bool connect();
    void run(); // TODO: start thread and input stream loop
};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
