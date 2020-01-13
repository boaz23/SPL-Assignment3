#include "../../include/api/ConnectionHandler.h"

#include <utility>

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(string host, short port):
    host_(std::move(host)), port_(port), io_service_(), socket_(io_service_),
    state_(State::Initialized), lock_() {}

std::string ConnectionHandler::host() const {
    return host_;
}
 
bool ConnectionHandler::connect() {
    if (!isClosed_doubledChecked()) {
        return false;
    }

    std::cout << "Starting connect to "
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
		state_ = State::Open;
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
    if (isClosed_doubledChecked()) {
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
    // TODO: synchronize
    if (isClosed_doubledChecked()) {
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
    if (isOpen_noLock()) {
        mutex_lock lock(lock_);
        close_noLock();
    }
}

bool ConnectionHandler::isOpen() {
    mutex_lock lock(lock_);
    return isOpen_noLock();
}

bool ConnectionHandler::isOpen_noLock() {
    return state_ == State::Open;
}

bool ConnectionHandler::isClosed() {
    return !isOpen();
}

bool ConnectionHandler::isClosed_doubledChecked() {
    if (isOpen_noLock()) {
        mutex_lock lock(lock_);
        return !isOpen_noLock();
    }

    return true;
}

void ConnectionHandler::close_noLock() {
    if (isOpen_noLock()) {
        try {
            socket_.shutdown(boost::asio::socket_base::shutdown_both);
            socket_.close();
            state_ = State::Closed;
        } catch (...) {
            std::cout << "connection closing failed" << std::endl;
        }
    }
}

ConnectionHandler::~ConnectionHandler() {
    close_noLock();
}
