#include <iostream>
#include <vector>
#include "../../include/Util.h"
#include "../../include/bookLibrary/Main.h"

using std::string;
using std::vector;

Main::Main(Printer &printer) :
    _printer(printer), _nextReceiptId(1), _nextSubscriptionId(1),
    _conn(nullptr), _encdec(nullptr), _activeUser(nullptr), _userThread(nullptr),
    _usersMap() { }

void Main::start() {
    // TODO: print using the printer
    while (true) {
        string command;
        getline(std::cin, command);
        if (command.empty()) {
            if (std::cin.eof()) {
                break;
            }
            else if (std::cin.fail()) {
                _printer.println("an unexpected error occured, input couldn't be interpreted");
                break;
            }
        }

        vector<string> arguments = Util::split(command, " ");
        if (arguments.empty()) {
            _printer.println("Error - no command entered");
            continue;
        }

        string action = arguments[0];
        if (action == "login") {
            if (_activeUser) {
                // TODO: ???
                _printer.println("cannot login when already logged-in.");
                continue;
            }

            login(arguments);
        }
        else {
            if (!_activeUser) {
                _printer.println("must be logged-in to perform that action.");
                continue;
            }
            if (_conn && _conn->isClosed()) {
                _printer.println("cannot perform that action because the connection is closed.");
                disconnectionCleanup();
                continue;
            }

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
            } else {
                _printer.println("Error - unknown action entered");
                continue;
            }
        }
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
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

    bool justAdded = initializeUser(host, port, username, password);
    connectAndRun(justAdded);
}
#pragma clang diagnostic pop

void Main::logout(const vector<string> &arguments) {
    if (arguments.size() != 1) {
        _printer.println("invalid usage of the logout command.");
        return;
    }

    disconnect();
    _userThread->join();
    disconnectionCleanup();
    // TODO: clear inventory
    _activeUser = nullptr;
}

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

bool Main::initializeUser(const string &host, short port, const string &username, const string &password) {
    bool justAdded;
    _encdec = new StompMessageEncoderDecoder();
    _conn = new StompConnectionHandler(host, port, _printer, *_encdec);
    if (_usersMap.count(username)) {
        _activeUser = _usersMap[username];
        justAdded = false;
    }
    else {
        _activeUser = new BookLibraryUser(username, password, _printer);
        _usersMap[username] = _activeUser;
        justAdded = true;
    }
    _activeUser->setConnection(_conn);
    _activeUser->setEncoderDecoder(_encdec);
    return justAdded;
}

void Main::connectAndRun(bool justAdded) {
    std::string err;
    if (_activeUser->connect(err)) {
        _printer.println("Login successful");
        _userThread = new std::thread(&BookLibraryUser::run, _activeUser);
    }
    else {
        _printer.println(err);
        _userThread = nullptr;
        disconnectionCleanup();
        if (justAdded) {
            std::unique_ptr<BookLibraryUser> userDeleter(_activeUser);
            _usersMap.erase(_activeUser->username());
            _activeUser = nullptr;
        }
    }
}

void Main::joinGenre(const std::string &genre, const std::string& subscriptionId) {
    std::string tmp;
    if (_activeUser->getSubscriptionIdFor(genre, tmp)) {
        return;
    }

    SubscribeFrame sendFrame(genre, subscriptionId);
    std::string receiptId = nextReceiptId();
    sendFrame.setReceiptId(receiptId);

    _activeUser->addSubscription(genre, subscriptionId);
    _activeUser->addReceipt(sendFrame);
    if (!_conn->sendFrame(sendFrame)) {
        _activeUser->removeSubscription(genre);
        _activeUser->removeReceipt(receiptId);
        _conn->close();
    }
}

void Main::exitGenre(const std::string &genre) {
    std::string subscriptionId;
    if (_activeUser->getSubscriptionIdFor(genre, subscriptionId)) {
        UnsubscribeFrame unsubscribeFrame(subscriptionId);
        unsubscribeFrame.setReceiptId(nextReceiptId());
        if (_conn->sendFrame(unsubscribeFrame)) {
            _activeUser->removeSubscription(genre);
        } else {
            _conn->close();
        }
    }
}

void Main::addBook(const std::string &genre, std::string &bookName) {
    UserBooks books = _activeUser->books();
    std::string bookGenre;
    if (books.getBookGenre(bookName, bookGenre) && genre != bookGenre) {
        _printer.println("book is already in the inventory in genre '" + bookGenre + "'");
    }
    else {
        SendFrame sendFrame(genre, _activeUser->username() + " has added the book " + bookName);
        if (_conn->sendFrame(sendFrame)) {
            books.addBook(genre, bookName);
        } else {
            _conn->close();
        }
    }
}

void Main::returnBook(const std::string &genre, const std::string &bookName) {
    std::string borrowedFrom;
    UserBooks books = _activeUser->books();
    if (books.getBorrowedFromUsername(genre, bookName, borrowedFrom)) {
        SendFrame sendFrame(genre, "Returning " + bookName + " to " + borrowedFrom);
        if (_conn->sendFrame(sendFrame)) {
            books.removeBorrowedBook(genre, bookName);
        } else {
            _conn->close();
        }
    }
    else {
        _printer.println("didn't borrow book '" + bookName + "'");
    }
}

void Main::borrowBook(const std::string &genre, const std::string &bookName) {
    SendFrame sendFrame(genre, _activeUser->username() + " wish to borrow " + bookName);
    if (!_conn->sendFrame(sendFrame)) {
        _conn->close();
    }
}

void Main::bookStatus(const std::string &genre) {
    SendFrame sendFrame(genre, "book status");
    if (!_conn->sendFrame(sendFrame)) {
        _conn->close();
    }
}

void Main::disconnect() {
    DisconnectFame disconnectFame;
    std::string receiptId = nextReceiptId();
    disconnectFame.setReceiptId(receiptId);
    _activeUser->addReceipt(disconnectFame);
    if (!_conn->sendFrame(disconnectFame)) {
        _activeUser->removeReceipt(receiptId);
        _conn->close();
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

void Main::disconnectionCleanup() {
    if (_activeUser) {
        _activeUser->books().clear();
    }
    std::unique_ptr<StompMessageEncoderDecoder> encdecDeleter(_encdec);
    std::unique_ptr<StompConnectionHandler> connectionDeleter(_conn);
    std::unique_ptr<std::thread> userThreadDeleter(_userThread);
    _userThread = nullptr;
    _conn = nullptr;
    _encdec = nullptr;
}
