
#ifndef STOMPCLIENT_BOOKLIBRARYUSER_H
#define STOMPCLIENT_BOOKLIBRARYUSER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "../Lock.h"
#include "UserBooks.h"
#include "../stomp/StompConnectionHandler.h"

class BookLibraryUser {
private:
    std::string _username;
    std::string _password;

    StompConnectionHandler &_connection;
    Printer &_printer;

    UserBooks _books;

    std::mutex _receiptsLock;
    std::unordered_map<std::string, Frame> receipts;

    std::unordered_map<std::string, std::string> genreToSubscriptionIds;
    std::unordered_map<std::string, std::string> subscriptionIdToGenres;
public:
    BookLibraryUser(std::string username, std::string password, StompConnectionHandler &connection, Printer &printer);
    BookLibraryUser(const BookLibraryUser &other);
    BookLibraryUser& operator=(const BookLibraryUser other);

    std::string username();
    UserBooks& books();

    bool connect(std::string &errorMsg);
    void run();

    void addReceipt(const Frame &frame);
    void removeReceipt(const std::string &receiptId);
    bool hasReceipt(const std::string &receiptId);
    Frame& getFrameForReceipt(const std::string &receiptId);
    void clearReceipts();

    void setSubscriptionId(const std::string &genre, const std::string &subscriptionId);
    void removeSubscription(const std::string &genre);
    bool getSubscriptionIdFor(const std::string &genre, std::string &subscriptionId);
    void clearSubscriptionMap();
private:
    bool sendHasBookFrame(const std::string &topic, const std::string &bookName);
    bool sendTakingBookFrom(const std::string &topic, const std::string &bookName, const std::string &from);
    bool sendSendFrame(const std::string &topic, const std::string &bookName);
    void printMessage(const std::string &topic, const std::string &body);

    bool sendBooksStatus(const std::string &dest);

    bool handlerSomeoneWantToBorrow(const std::string &dest, const std::vector<std::string> &message);
    bool handlerUserHasBook(const std::string &dest, const std::vector<std::string> &message);
    void handleSomeoneTakingBookFrom(const std::string &dest, const std::vector<std::string> &message);
    void returnedBook(const std::string &dest, const std::vector<std::string> &message);

#ifdef DEBUG_PRINT_FRAMES
    void debugPrintFrame(const Frame &frame);
#endif
};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
