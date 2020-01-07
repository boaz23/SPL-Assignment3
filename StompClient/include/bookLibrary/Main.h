
#ifndef STOMPCLIENT_MAIN_H
#define STOMPCLIENT_MAIN_H

#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include "UserBooks.h"
#include "../Printer.h"
#include "../stomp/Frame.h"
#include "../stomp/StompMessageEncoderDecoder.h"
#include "../stomp/StompConnectionHandler.h"
#include "BookLibraryUser.h"

// TODO: implement rule of 5
class Main {
private:
    std::string _host;
    int _port;

    Printer &_printer;

    // TODO: maybe change to pointers and initialize with null
    StompConnectionHandler *_conn;
    StompMessageEncoderDecoder *_encdec;
    BookLibraryUser *_user;
    std::thread *_userThread;
public:
    Main(std::string host, int port, Printer &printer);
    void start();

private:
    std::vector<std::string> splitStringBySpace(std::string s);
};


#endif //STOMPCLIENT_MAIN_H
