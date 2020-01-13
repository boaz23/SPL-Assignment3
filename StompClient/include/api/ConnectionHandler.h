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

	const std::string host_;
	const short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	State state_;

	std::mutex lock_;
public:
    ConnectionHandler(std::string host, short port);

    std::string host() const;
 
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