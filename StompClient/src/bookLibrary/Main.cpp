#include <iostream>
#include <vector>
#include "../../include/bookLibrary/Main.h"
#include "../../include/api/ConnectionHandler.h"
#include "../../include/stomp/StompMessageEncoderDecoder.h"

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
            _encdec = new StompMessageEncoderDecoder();
            _conn = new StompConnectionHandler(_host, _port, _printer, *_encdec);
            _activeUser = new BookLibraryUser(arguments[1], *_conn, *_encdec);

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
