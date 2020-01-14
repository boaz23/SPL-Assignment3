#include <iostream>
#include <vector>
#include "../../include/Util.h"
#include "../../include/bookLibrary/Main.h"

using std::string;
using std::vector;

Main::Main(StompMessageEncoderDecoder &encdec, StompConnectionHandler &connection, Printer &printer) :
    _quit(false),
    _encdec(encdec), _connection(connection), _printer(printer),
    _nextReceiptId(1), _nextSubscriptionId(1),
    _users(), _userThread() { }

Main::~Main() {
    disconnectionCleanup();
}

void Main::start() {
    while (!_quit) {
        string cmd;
        if (!readCommand(cmd)) {
            break;
        }

        vector<string> arguments;
        if (!getArguments(cmd, arguments)) {
            continue;
        }

        handleCommand(arguments);
    }
}

void Main::login(const vector<string> &arguments) {
    if (arguments.size() != 4) {
        _printer.println("invalid usage of the login command.");
        return;
    }

    std::string sServer = arguments[1];
    vector<string> vServer = Util::split(sServer, ":");
    string host = vServer[0];
    auto port = (short)atoi(vServer[1].c_str());

    std::string username = arguments[2];
    std::string password = arguments[3];

    bool added = initializeUser(username, password);
    connectAndRun(host, port, added);
}

void Main::logout(const vector<string> &arguments) {
    if (arguments.size() != 1) {
        _printer.println("invalid usage of the logout command.");
        return;
    }

    disconnect();
    disconnectionCleanup();
}

bool Main::initializeUser(const std::string &username, const std::string &password) {
    bool added;
    if (_users.doesUserExists(username)) {
        _users.setActiveUser(username);
        added = false;
    }
    else {
        _users.addUserAndSetAsActive(username, password, _connection, _printer);
        added = true;
    }
    return added;
}

void Main::connectAndRun(const std::string &host, const short port, const bool added) {
    std::string err;
    _connection.setServer(host, port);
    if (_connection.connect() && activeUser().connect(err)) {
        _printer.println("Login successful");
        std::thread tmpThread(&BookLibraryUser::run, _users.activeUserPtr());
        _userThread = std::move(tmpThread);
    }
    else {
        _printer.println(err);
        if (added) {
            _users.removeActiveUser();
        }
        disconnectionCleanup();
    }
}

void Main::disconnect() {
    DisconnectFame disconnectFame;
    std::string receiptId = nextReceiptId();
    disconnectFame.setReceiptId(receiptId);
    activeUser().addReceipt(disconnectFame);
    if (!_connection.sendFrame(disconnectFame)) {
        activeUser().removeReceipt(receiptId);
        _connection.close();
    }
}

void Main::disconnectionCleanup() {
    cleanupUser();
    cleanupConnection();
}

void Main::cleanupUser() {
    if (_users.isUserActive()) {
        activeUser().books().clear();
        activeUser().clearReceipts();
        activeUser().clearSubscriptionMap();
        _users.setNoUserActive();
    }
    if (_userThread.joinable()) {
        _userThread.join();
        _userThread = std::thread();
    }
    _nextReceiptId = 1;
    _nextSubscriptionId = 1;
}

void Main::cleanupConnection() { }

void Main::joinGenre(const std::vector<std::string> &arguments) {
    if (arguments.size() != 2) {
        _printer.println("invalid usage of the join genre command.");
        return;
    }

    std::string genre = arguments[1];
    std::string subscriptionId = nextSubscriptionId();
    joinGenre(genre, subscriptionId);
}

void Main::exitGenre(const std::vector<std::string> &arguments) {
    if (arguments.size() != 2) {
        _printer.println("invalid usage of the exit genre command.");
        return;
    }

    std::string genre = arguments[1];
    exitGenre(genre);
}

void Main::addBook(const std::vector<std::string> &arguments) {
    if (arguments.size() != 3) {
        _printer.println("invalid usage of the add book command.");
        return;
    }

    std::string genre = arguments[1];
    std::string bookName = arguments[2];
    addBook(genre, bookName);
}

void Main::borrowBook(const std::vector<std::string> &arguments) {
    if (arguments.size() != 3) {
        _printer.println("invalid usage of the borrow command.");
        return;
    }

    std::string genre = arguments[1];
    std::string bookName = arguments[2];
    borrowBook(genre, bookName);
}

void Main::returnBook(const std::vector<std::string> &arguments) {
    if (arguments.size() != 3) {
        _printer.println("invalid usage of the return command.");
        return;
    }

    std::string genre = arguments[1];
    std::string bookName = arguments[2];
    returnBook(genre, bookName);
}

void Main::bookStatus(const std::vector<std::string> &arguments) {
    if (arguments.size() != 2) {
        _printer.println("invalid usage of the book status command.");
        return;
    }

    std::string genre = arguments[1];
    bookStatus(genre);
}

void Main::joinGenre(const std::string &genre, const std::string& subscriptionId) {
    SubscribeFrame sendFrame(genre, subscriptionId);
    std::string receiptId = nextReceiptId();
    sendFrame.setReceiptId(receiptId);

    std::string tmp;
    if (!activeUser().getSubscriptionIdFor(genre, tmp)) {
        activeUser().setSubscriptionId(genre, subscriptionId);
    }
    activeUser().addReceipt(sendFrame);
    if (!_connection.sendFrame(sendFrame)) {
        activeUser().removeSubscription(genre);
        activeUser().removeReceipt(receiptId);
        _connection.close();
    }
}

void Main::exitGenre(const std::string &genre) {
    std::string subscriptionId;
    if (activeUser().getSubscriptionIdFor(genre, subscriptionId)) {
        UnsubscribeFrame unsubscribeFrame(subscriptionId);
        std::string receiptId = nextReceiptId();
        unsubscribeFrame.setReceiptId(receiptId);
        activeUser().addReceipt(unsubscribeFrame);
        if (!_connection.sendFrame(unsubscribeFrame)) {
            activeUser().removeSubscription(genre);
            activeUser().removeReceipt(receiptId);
            _connection.close();
        }
    }
}

void Main::addBook(const std::string &genre, std::string &bookName) {
    UserBooks &books = activeUser().books();
    std::string bookGenre;
    if (books.getBookGenre(bookName, bookGenre) && genre != bookGenre) {
        _printer.println("book is already in the inventory in genre '" + bookGenre + "'");
    }
    else {
        SendFrame sendFrame(genre, activeUser().username() + " has added the book " + bookName);
        if (_connection.sendFrame(sendFrame)) {
            books.addBook(genre, bookName);
        } else {
            _connection.close();
        }
    }
}

void Main::returnBook(const std::string &genre, const std::string &bookName) {
    std::string borrowedFrom;
    UserBooks &books = activeUser().books();
    if (books.getBorrowedFromUsername(genre, bookName, borrowedFrom)) {
        SendFrame sendFrame(genre, "Returning " + bookName + " to " + borrowedFrom);
        if (_connection.sendFrame(sendFrame)) {
            books.removeBorrowedBook(genre, bookName);
        } else {
            _connection.close();
        }
    }
    else {
        _printer.println("didn't borrow book '" + bookName + "'");
    }
}

void Main::borrowBook(const std::string &genre, const std::string &bookName) {
    SendFrame sendFrame(genre, activeUser().username() + " wish to borrow " + bookName);
    if (_connection.sendFrame(sendFrame)) {
        activeUser().books().addBookAsWantToBorrow(genre, bookName);
    } else {
        _connection.close();
    }
}

void Main::bookStatus(const std::string &genre) {
    SendFrame sendFrame(genre, "book status");
    if (!_connection.sendFrame(sendFrame)) {
        _connection.close();
    }
}

std::string Main::getBookName(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end) {
    std::string bookName;
    auto word = start;
    bookName.append(*word);
    ++word;
    for (; word != end; ++word) {
        bookName.append(" ")
            .append(*word);
    }

    return bookName;
}

std::string Main::nextReceiptId() {
    return nextId(_nextReceiptId);
}

std::string Main::nextSubscriptionId() {
    return nextId(_nextSubscriptionId);
}

template <typename T> std::string Main::nextId(T &id) {
    return std::to_string(id++);
}

BookLibraryUser& Main::activeUser() {
    return _users.activeUser();
}

bool Main::readCommand(std::string &cmd) {
    getline(std::cin, cmd);
    if (cmd.empty()) {
        if (std::cin.eof()) {
            return false;
        }
        else if (std::cin.fail()) {
            _printer.println("an unexpected error occured, input couldn't be interpreted");
            return false;
        }
    }

    return true;
}

bool Main::getArguments(const std::string &cmd, std::vector<std::string> &arguments) {
    arguments = Util::split(cmd, " ");
    if (arguments.empty()) {
        _printer.println("Error - no command entered");
        return false;
    }

    return true;
}

bool Main::handleCommand(const std::vector<std::string> &arguments) {
    string action = arguments[0];
    if (action == "login") {
        return handleLoginCommand(arguments);
    }
    else {
        return handleNonLoginCommand(arguments);
    }
}

bool Main::handleLoginCommand(const vector<std::string> &arguments) {
    if (_connection.isClosed()) {
        disconnectionCleanup();
    }
    else if (_users.isUserActive()) {
        // TODO: ???
        _printer.println("cannot login when already logged-in.");
        return false;
    }

    login(arguments);
    return true;
}

bool Main::handleNonLoginCommand(const std::vector<std::string> &arguments) {
    if (!_users.isUserActive()) {
        _printer.println("must be logged-in to perform that action.");
        return false;
    }
    if (_connection.isClosed()) {
        disconnectionCleanup();
        return handleCommand(arguments);
    }

    return invokeCommand(arguments);
}

bool Main::invokeCommand(const std::vector<std::string> &arguments) {
    string action = arguments[0];
    if (action == "join") {
        joinGenre(arguments);
    } else if (action == "exit") {
        exitGenre(arguments);
    } else if (action == "add") {
        addBook(arguments);
    } else if (action == "borrow") {
        borrowBook(arguments);
    } else if (action == "return") {
        returnBook(arguments);
    } else if (action == "status") {
        bookStatus(arguments);
    } else if (action == "logout") {
        logout(arguments);
    } else if (action == "quit") {
        _quit = true;
    } else {
        _printer.println("Error - unknown action entered");
        return false;
    }

    return true;
}

UserHolder::UserHolder() : _activeUser(nullptr), _usersMap() { }
UserHolder::UserHolder(const UserHolder &other) : _activeUser(nullptr), _usersMap(other._usersMap) {
    setActiveUser(other);
}
UserHolder::UserHolder(UserHolder &&other) noexcept : _activeUser(nullptr), _usersMap(std::move(other._usersMap)) {
    setActiveUser(other);
}
UserHolder& UserHolder::operator=(const UserHolder &other) {
    if (&other != this) {
        clean();
        _usersMap = other._usersMap;
        setActiveUser(other);
    }

    return *this;
}
UserHolder& UserHolder::operator=(UserHolder &&other) noexcept {
    if (&other != this) {
        clean();
        _usersMap = std::move(other._usersMap);
        setActiveUser(other);
    }

    return *this;
}

UserHolder::~UserHolder() {
    clean();
}

void UserHolder::setActiveUser(const UserHolder &other) {
    if (other._activeUser) {
        BookLibraryUser &user = _usersMap.at(other._activeUser->username());
        _activeUser = &user;
    }
}

void UserHolder::clean() {
    _activeUser = nullptr;
}

BookLibraryUser& UserHolder::activeUser() {
    return *_activeUser;
}
BookLibraryUser * UserHolder::activeUserPtr() {
    return _activeUser;
}

bool UserHolder::doesUserExists(const std::string &username) {
    return _usersMap.count(username);
}

void UserHolder::addUserAndSetAsActive(const std::string &username, const std::string &password, StompConnectionHandler &connection, Printer &printer) {
    BookLibraryUser user(username, password, connection, printer);
    _usersMap.emplace(username, user);
    setActiveUser(username);
}

void UserHolder::setActiveUser(const std::string &username) {
    _activeUser = &_usersMap.at(username);
}

void UserHolder::removeActiveUser() {
    _usersMap.erase(_activeUser->username());
    _activeUser = nullptr;
}

void UserHolder::setNoUserActive() {
    _activeUser = nullptr;
}

bool UserHolder::isUserActive() {
    return _activeUser;
}