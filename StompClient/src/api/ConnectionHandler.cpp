#include "../../include/api/ConnectionHandler.h"

#include <utility>

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

// TODO: use printer for printing
// TODO: synchronize with mutex and lock_guard
ConnectionHandler::ConnectionHandler(string host, short port, Printer &printer):
    host_(std::move(host)), port_(port), io_service_(), socket_(io_service_),
    closed_(false), _printer(printer) {}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
        return true;
    }
    catch (boost::system::system_error& e) {
        std::cerr << "Could not connect to server" << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
    }

    return false;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    if (closed_) {
        return false;
    }

    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
        return true;
    }
    catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
    }

    return false;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    if (closed_) {
        return false;
    }

    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    if (!isClosed()) {
        close_noLock();
    }
}

std::string ConnectionHandler::host() const {
    return host_;
}

bool ConnectionHandler::isClosed() {
    return closed_;
}

void ConnectionHandler::close_noLock() {
    if (!isClosed()) {
        try {
            socket_.shutdown(boost::asio::socket_base::shutdown_both);
            socket_.close();
            closed_ = true;
        } catch (...) {
            std::cout << "closing failed: connection already closed" << std::endl;
        }
    }
}

ConnectionHandler::~ConnectionHandler() {
    close_noLock();
}