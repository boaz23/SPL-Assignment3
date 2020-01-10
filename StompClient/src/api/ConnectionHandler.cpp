#include "../../include/api/ConnectionHandler.h"

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

// TODO: use printer for printing
// TODO: synchronize with mutex and lock_guard
ConnectionHandler::ConnectionHandler(string host, short port, Printer &printer):
    host_(host), port_(port), io_service_(), socket_(io_service_),
    closed_(false), _printer(printer) {}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
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

//bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
//    char ch;
//    // Stop when we encounter the null character.
//    // Notice that the null character is not appended to the Frame string.
//    try {
//	do{
//		if(!getBytes(&ch, 1))
//		{
//			return false;
//		}
//		if(ch!='\0')
//			frame.append(1, ch);
//	}while (delimiter != ch);
//    } catch (std::exception& e) {
//	std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
//	return false;
//    }
//    return true;
//}
//
//
//bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
//	bool result=sendBytes(frame.c_str(),frame.length());
//	if(!result) return false;
//	return sendBytes(&delimiter,1);
//}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    if (!closed_) {
        try {
            socket_.shutdown(boost::asio::socket_base::shutdown_both);
            socket_.close();
            closed_ = true;
        } catch (...) {
            std::cout << "closing failed: connection already closed" << std::endl;
        }
    }
}

std::string ConnectionHandler::host() const {
    return host_;
}

bool ConnectionHandler::isClosed() {
    return closed_;
}