
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

    UserBooks _books;
    std::unordered_map<std::string, Frame> receipts;
    std::unordered_map<std::string, std::string> genreToSubscriptionIds;

    std::unordered_map<std::string, std::string> pendingBorrows;
    std::unordered_map<std::string, std::string> successfulBorrows;
public:
    BookLibraryUser(std::string username, std::string password, Printer &printer);

    std::string username();
    UserBooks& books() const;

    bool connect(std::string &errorMsg);
    void run();

    void addReceipt(const Frame &frame);
    void removeReceipt(const std::string &receiptId);

    void addSubscription(std::string genre, std::string subscriptionId);
    void removeSubscription(std::string genre);
    bool getSubscriptionIdFor(std::string genre, std::string &subscriptionId);

    void setConnection(StompConnectionHandler *connection);
    void setEncoderDecoder(StompMessageEncoderDecoder *encdec);
private:
    bool readFrame(std::unique_ptr<Frame> &frame);

    bool sendHasBookFrame(const std::string &topic, const std::string &bookName);
    bool sendTakingBookFrom(const std::string &topic, const std::string &bookName, const std::string &from);
    bool sendSendFrame(const std::string &topic, const std::string &bookName);
    void printMessage(const std::string &topic, const std::string &body);

    bool sendBooksStatus(const std::string &dest);

    bool handlerSomeoneWantToBorrow(const std::string &dest, const std::vector<std::string> &message);
    bool handlerUserHasBook(const std::string &dest, const std::vector<std::string> &message);
    void handleSomeoneTakingBookFrom(const std::string &dest, const std::vector<std::string> &message);
    void returnedBook(const std::string &dest, const std::vector<std::string> &message);

};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
