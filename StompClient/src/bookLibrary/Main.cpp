#include <iostream>
#include <vector>
#include "../../include/bookLibrary/Main.h"

using std::string;
using std::vector;

Main::Main(Printer &printer) :
    _printer(printer),
    _conn(nullptr), _encdec(nullptr), _activeUser(nullptr), _userThread(nullptr),
    _usersMap() { }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
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
                std::cerr << "an unexpected error occured, input couldn't be interpreted";
                break;
            }
        }

        vector<string> arguments = split(command, " ");
        if (arguments.empty()) {
            std::cout << "Error - no command entered" << std::endl;
            continue;
        }

        string action = arguments[0];
        if (action == "login") {
            if (arguments.size() != 4) {
                _printer.println("invalid usage of login action.");
            }

            std::string sServer = arguments[1];
            vector<string> vServer = split(sServer, ":");
            string host = vServer[0];
            auto port = (short)atoi(vServer[1].c_str());

            std::string username = arguments[2];
            std::string password = arguments[3];

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

            std::string err;
            if (_activeUser->connect(err)) {
                _printer.println("Login successful");
                _userThread = new std::thread(&BookLibraryUser::run, _activeUser);
            }
            else {
                _printer.println(err);
                std::unique_ptr<StompMessageEncoderDecoder> encdecDeleter(_encdec);
                std::unique_ptr<StompConnectionHandler> connectionDeleter(_conn);
                if (justAdded) {
                    std::unique_ptr<BookLibraryUser> userDeleter(_activeUser);
                    _usersMap.erase(username);
                }
            }
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
            // TODO: delete all resources
        }
        else {
            std::cout << "Error - unknown action entered" << std::endl;
            continue;
        }
    }
}
#pragma clang diagnostic pop

vector<string> split(const string &s, const string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}