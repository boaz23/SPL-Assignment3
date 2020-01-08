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
            // TODO: handle the case when already connected
            if (arguments.size() != 3) {
                _printer.println("invalid usage of login action.");
            }

            std::string username = arguments[1];
            std::string password = arguments[2];
            if (_usersMap.count(username) > 0) {
                _activeUser = _usersMap[username];
            }
            else {
                _encdec = new StompMessageEncoderDecoder();
                _conn = new StompConnectionHandler(_host, (short)_port, _printer, *_encdec);

                _activeUser = new BookLibraryUser(username, password, *_conn, *_encdec);
                _usersMap[username] = _activeUser;
                _printer.println("Login successful");
            }

            if (!_activeUser->connect()) {
                // TODO: delete resources
            }

            // TODO: start thread;
            _userThread = new std::thread(&BookLibraryUser::run, _activeUser);

            // TODO: this is pseduo code, refactor this
//            array<byte> bytes = encdec.encode(ConnectFrame()); // TODO: delete the bytes array object
//            connectionHandler.sendBytes(bytes.array, bytes.length);
//            byte b;
//            while (!connectMessageEnd() && !connectionHandler.getBytes(&b, 1)) {
//                Frame* frame = encdec.decodeNextByte(b); // TODO: use smart pointer
//                if (frame) {
//                    // TODO: parse CONNECTED frame,
//                    //  if valid, break from the loop and start a new thread which reads frames from the server
//                }
//            }
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
