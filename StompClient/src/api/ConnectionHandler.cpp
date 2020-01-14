#include "../../include/api/ConnectionHandler.h"

#include <utility>

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler():
    _host(), _port(), _io_service(), _socket(_io_service),
    _state(State::Initialized), _lock() {}

std::string ConnectionHandler::host() const {
    return _host;
}

short ConnectionHandler::port() const {
    return _port;
}

bool ConnectionHandler::setServer(const std::string &host, const short port) {
    if (!isClosed_doubledChecked()) {
        return false;
    }

    _host = host;
    _port = port;
    return true;
}
 
bool ConnectionHandler::connect() {
    if (!isClosed_doubledChecked()) {
        return false;
    }

    std::cout << "Starting connect to "
        << _host << ":" << _port << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(_host), _port); // the server endpoint
		boost::system::error_code error;
		_socket.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
        _state = State::Open;
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
			tmp += _socket.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
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
    if (!isOpen_noLock()) {
        return false;
    }
    mutex_lock lock(_lock);
    if (!isOpen_noLock()) {
        return false;
    }

    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += _socket.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
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
        mutex_lock lock(_lock);
        close_noLock();
    }
}

bool ConnectionHandler::isOpen() {
    mutex_lock lock(_lock);
    return isOpen_noLock();
}

bool ConnectionHandler::isOpen_noLock() {
    return _state == State::Open;
}

bool ConnectionHandler::isClosed() {
    return !isOpen();
}

bool ConnectionHandler::isClosed_doubledChecked() {
    if (isOpen_noLock()) {
        mutex_lock lock(_lock);
        return !isOpen_noLock();
    }

    return true;
}

void ConnectionHandler::close_noLock() {
    if (isOpen_noLock()) {
        try {
            _socket.shutdown(boost::asio::socket_base::shutdown_both);
            _socket.close();
            _state = State::Closed;
        } catch (...) {
            std::cout << "connection closing failed" << std::endl;
        }
    }
}

ConnectionHandler::~ConnectionHandler() {
    close_noLock();
}
