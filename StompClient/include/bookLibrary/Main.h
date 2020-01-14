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

// TODO: think of a way to not hold pointers
class UserHolder {
private:
    BookLibraryUser *_activeUser;
    std::unordered_map<std::string, BookLibraryUser> _usersMap;

public:
    UserHolder();
    UserHolder(const UserHolder &other);
    UserHolder(UserHolder &&other) noexcept;
    UserHolder& operator=(const UserHolder &other);
    UserHolder& operator=(UserHolder &&other) noexcept;
    ~UserHolder();

    BookLibraryUser& activeUser();
    BookLibraryUser* activeUserPtr();
    bool doesUserExists(const std::string &username);
    void addUserAndSetAsActive(const std::string &username, const std::string &password, StompConnectionHandler &connection, Printer &printer);
    void setActiveUser(const std::string &username);
    void removeActiveUser();
    void setNoUserActive();
    bool isUserActive();

private:
    void clean();
    void setActiveUser(const UserHolder &other);
};

class Main {
private:
    StompMessageEncoderDecoder &_encdec;
    StompConnectionHandler &_connection;
    Printer &_printer;

    int _nextReceiptId;
    int _nextSubscriptionId;
    UserHolder _users;
    std::thread _userThread;
public:
    Main(StompMessageEncoderDecoder &encdec, StompConnectionHandler &connection, Printer &printer);
    ~Main();
    void start();

private:
    void login(const std::vector<std::string> &arguments);
    bool initializeUser(const std::string &username, const std::string &password);
    void connectAndRun(const std::string &host, const short port, const bool added);

    void logout(const std::vector<std::string> &arguments);
    void disconnect();
    void disconnectionCleanup();
    void cleanupUser();
    void cleanupConnection();

    void joinGenre(const std::vector<std::string> &arguments);
    void joinGenre(const std::string &genre, const std::string& subscriptionId);

    void exitGenre(const std::vector<std::string> &arguments);
    void exitGenre(const std::string &genre);

    void addBook(const std::vector<std::string> &arguments);
    void addBook(const std::string &genre, std::string &bookName);

    void borrowBook(const std::vector<std::string> &arguments);
    void borrowBook(const std::string &genre, const std::string &bookName);

    void returnBook(const std::vector<std::string> &arguments);
    void returnBook(const std::string &genre, const std::string &bookName);

    void bookStatus(const std::vector<std::string> &arguments);
    void bookStatus(const std::string &genre);

    std::string getBookName(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end);

    std::string nextReceiptId();
    std::string nextSubscriptionId();
    template <typename T> std::string nextId(T &id);
    BookLibraryUser& activeUser();

    bool readCommand(std::string &cmd);
    bool getArguments(const std::string &cmd, std::vector<std::string> &arguments);
    bool handleCommand(const std::vector<std::string> &arguments);
    bool handleLoginCommand(const std::vector<std::string> &arguments);
    bool handleNonLoginCommand(const std::vector<std::string> &arguments);
    bool invokeCommand(const std::vector<std::string> &arguments);
};


#endif //STOMPCLIENT_MAIN_H
