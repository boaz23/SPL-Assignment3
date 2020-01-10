#include <iostream>
#include <vector>
#include "../../include/Util.h"
#include "../../include/bookLibrary/Main.h"

using std::string;
using std::vector;

Main::Main(Printer &printer) :
    _printer(printer), _nextReceiptId(1),
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
            login(arguments);
        }
        else if (action == "join") {

        }
        else if (action == "exit") {

        }
        else if (action == "add") {

        }
        else if (action == "borrow") {

        }
        else if (action == "return") {

        }
        else if (action == "status") {

        }
        else if (action == "logout") {
            logout(arguments);
        }
        else {
            std::cout << "Error - unknown action entered" << std::endl;
            continue;
        }
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
void Main::login(const vector<string> &arguments) {
    if (arguments.size() != 4) {
        _printer.println("invalid usage of login action.");
        return;
    }

    std::string sServer = arguments[1];
    vector<string> vServer = Util::split(sServer, ":");
    string host = vServer[0];
    auto port = (short)atoi(vServer[1].c_str());

    std::string username = arguments[2];
    std::string password = arguments[3];

    if (!_activeUser) {
        bool justAdded = initializeUser(host, port, username, password);
        connectAndRun(justAdded);
    }
    else {
        // TODO: ???
    }
}
#pragma clang diagnostic pop

void Main::logout(const vector<string> &arguments) {
    if (arguments.size() != 1) {
        _printer.println("invalid usage of logout action.");
        return;
    }

    if (!_activeUser) {
        _printer.println("must be logged");
        return;
    }

    disconnect();
    _userThread->join();
    disconnectionCleanup();
    // TODO: clear inventory
    _activeUser = nullptr;
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

void Main::disconnect() {
    DisconnectFame disconnectFame;
    std::string receiptId = nextReceiptId();
    disconnectFame.setReceiptId(receiptId);
    _activeUser->addReceipt(disconnectFame);
    if (!_conn->sendFrame(disconnectFame)) {
        _activeUser->removeReceipt(receiptId);
        _conn->close(); // Interrupt the reading thread
    }
}

std::string Main::nextReceiptId() {
    return std::to_string(_nextReceiptId++);
}

void Main::disconnectionCleanup() {
    std::unique_ptr<StompMessageEncoderDecoder> encdecDeleter(_encdec);
    std::unique_ptr<StompConnectionHandler> connectionDeleter(_conn);
    std::unique_ptr<std::thread> userThreadDeleter(_userThread);
    _userThread = nullptr;
    _conn = nullptr;
    _encdec = nullptr;
}
