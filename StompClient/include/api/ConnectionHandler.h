#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "../Printer.h"
#include "../Lock.h"

using boost::asio::ip::tcp;

class ConnectionHandler {
private:
    enum class State {
        Initialized,
        Open,
        Closed,
    };

	std::string _host;
	short _port;
	boost::asio::io_service _io_service;   // Provides core I/O functionality
	tcp::socket _socket;
	State _state;

	std::mutex _lock;
public:
    ConnectionHandler();

    std::string host() const;
    short port() const;

    bool setServer(const std::string &host, const short port);
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);
 
	// Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);
	
    // Close down the connection properly.
    void close();

    bool isClosed();
    bool isOpen();
    virtual ~ConnectionHandler();

private:
    void close_noLock();
    bool isClosed_doubledChecked();
    bool isOpen_noLock();
}; //class ConnectionHandler
 
#endif