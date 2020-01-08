#include <iostream>
#include <vector>
#include "../../include/bookLibrary/Main.h"

using std::string;
using std::vector;

Main::Main(std::string host, int port, Printer &printer) : _host(host), _port(port), _printer(printer),
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
                std::cerr << "an unexpected error occured, input couldn't be interpreted";
                break;
            }
        }

        vector<string> arguments = splitStringBySpace(command);
        if (arguments.empty()) {
            std::cout << "Error - no command entered" << std::endl;
            continue;
        }

        string action = arguments[0];
        if (action == "login") {
            if (arguments.size() != 3) {
                _printer.println("invalid usage of login action.");
            }

            std::string username = arguments[1];
            std::string password = arguments[2];
            bool justAdded;
            if (_usersMap.count(username) > 0) {
                _activeUser = _usersMap[username];
                justAdded = false;
            }
            else {
                _encdec = new StompMessageEncoderDecoder();
                _conn = new StompConnectionHandler(_host, (short)_port, _printer, *_encdec);

                _activeUser = new BookLibraryUser(username, password, *_conn, *_encdec);
                _usersMap[username] = _activeUser;
                justAdded = true;
            }

            std::string err;
            if (_activeUser->connect(err)) {
                _printer.println("Login successful");
                _userThread = new std::thread(&BookLibraryUser::run, _activeUser);
            }
            else {
                _printer(err);
                if (justAdded) {
                    std::unique_ptr<BookLibraryUser> userDeleter(_activeUser);
                    _usersMap.erase(username);
                }

                std::unique_ptr<StompMessageEncoderDecoder> encdecDeleter;
                std::unique_ptr<StompConnectionHandler> connectionDeleter;
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
