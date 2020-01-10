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
    Printer &_printer;
    int _nextReceiptId;

    // TODO: maybe change to pointers and initialize with null
    StompConnectionHandler *_conn;
    StompMessageEncoderDecoder *_encdec;
    BookLibraryUser *_activeUser;
    std::thread *_userThread;
    std::unordered_map<std::string, BookLibraryUser*> _usersMap;
public:
    Main(Printer &printer);
    void start();

private:
    void login(const std::vector<std::string> &arguments);
    bool initializeUser(const std::string &host, short port, const std::string &username, const std::string &password);
    void connectAndRun(bool justAdded);

    void logout(const std::vector<std::string> &arguments);
    void disconnect();

    void borrow(const std::vector<std::string> &arguments);

    std::string getBookName(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end);

    std::string nextReceiptId();
    void disconnectionCleanup();
};


#endif //STOMPCLIENT_MAIN_H
