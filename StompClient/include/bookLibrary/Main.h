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
    int _nextSubscriptionId;

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

    void join(const std::vector<std::string> &arguments);
    void join(const std::string &genre, const std::string& subscriptionId);

    void exit(const std::vector<std::string> &arguments);
    void exit(const std::string &genre);

    void borrow(const std::vector<std::string> &arguments);
    void borrow(const std::string &genre, const std::string &bookName);

    void bookStatus(const std::vector<std::string> &arguments);
    void bookStatus(const std::string &genre);

    std::string getBookName(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end);

    std::string nextReceiptId();
    std::string nextSubscriptionId();
    template <typename T> std::string nextId(T &id);

    void disconnectionCleanup();
};


#endif //STOMPCLIENT_MAIN_H
