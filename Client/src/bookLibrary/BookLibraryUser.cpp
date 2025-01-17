
#include "../../include/bookLibrary/BookLibraryUser.h"

#include <utility>
#include "../../include/stomp/StompVersion.h"
#include "../../include/Util.h"

BookLibraryUser::BookLibraryUser(
    std::string username, std::string password,
    StompConnectionHandler &connection, Printer &printer
) : _username(std::move(username)), _password(std::move(password)),
    _connection(connection), _printer(printer),
    _books(), _receiptsLock(), receipts(), genreToSubscriptionIds(), subscriptionIdToGenres() {}

BookLibraryUser::BookLibraryUser(const BookLibraryUser &other) :
_username(other._username), _password(other._password),
_connection(other._connection), _printer(other._printer),
_books(other._books), _receiptsLock(), receipts(other.receipts), genreToSubscriptionIds(other.genreToSubscriptionIds), subscriptionIdToGenres(other.subscriptionIdToGenres) { }

BookLibraryUser & BookLibraryUser::operator=(const BookLibraryUser other) {
    if (&other != this) {
        _username = other._username;
        _password = other._password;
        _books = other._books;
        receipts = other.receipts;
        genreToSubscriptionIds = other.genreToSubscriptionIds;
        subscriptionIdToGenres = other.subscriptionIdToGenres;
    }

    return *this;
}

std::string BookLibraryUser::username() {
    return _username;
}

UserBooks &BookLibraryUser::books() {
    return _books;
}

void BookLibraryUser::addReceipt(const Frame &frame) {
    mutex_lock lock(_receiptsLock);
    receipts[frame.receipt()] = frame;
}

void BookLibraryUser::removeReceipt(const std::string &receiptId) {
    mutex_lock lock(_receiptsLock);
    receipts.erase(receiptId);
}

bool BookLibraryUser::hasReceipt(const std::string &receiptId) {
    mutex_lock lock(_receiptsLock);
    return receipts.count(receiptId);
}

Frame &BookLibraryUser::getFrameForReceipt(const std::string &receiptId) {
    mutex_lock lock(_receiptsLock);
    return receipts.at(receiptId);
}

void BookLibraryUser::clearReceipts() {
    receipts.clear();
}

void BookLibraryUser::setSubscriptionId(const std::string &genre, const std::string &subscriptionId) {
    genreToSubscriptionIds[genre] = subscriptionId;
    subscriptionIdToGenres[subscriptionId] = genre;
}

void BookLibraryUser::removeSubscription(const std::string &genre) {
    std::string subscriptionId = genreToSubscriptionIds.at(genre);
    genreToSubscriptionIds.erase(genre);
    subscriptionIdToGenres.erase(subscriptionId);
}

bool BookLibraryUser::getSubscriptionIdFor(const std::string &genre, std::string &subscriptionId) {
    if (genreToSubscriptionIds.count(genre)) {
        subscriptionId = genreToSubscriptionIds[genre];
        return true;
    }

    return false;
}

void BookLibraryUser::clearSubscriptionMap() {
    genreToSubscriptionIds.clear();
}

bool BookLibraryUser::connect(std::string &errorMsg) {
    ConnectFrame connectFrame(ACCEPT_VERSION, _connection.host(), _username, _password);
    if (!_connection.sendFrame(connectFrame)) {
        return false;
    }

    std::unique_ptr<Frame> frame = nullptr;
    if (!_connection.readFrame(frame)) {
        return false;
    }

    const std::string &messageType = frame->messageType();
    if (messageType == ConnectedFrame::MESSAGE_TYPE) {
        auto *connectedFrame = dynamic_cast<ConnectedFrame*>(frame.get());
        if (connectedFrame->version() != ACCEPT_VERSION) {
            return false;
        }
    }
    else {
        if (messageType == ErrorFrame::MESSAGE_TYPE) {
            auto *errorFrame = dynamic_cast<ErrorFrame*>(frame.get());
            errorMsg = errorFrame->errorMessage();
        }

        return false;
    }

    return true;
}

void BookLibraryUser::run() {
    std::unique_ptr<Frame> frame = nullptr;

    while(true) {
        if (!_connection.readFrame(frame)) {
            _connection.close();
            break;
        }

#ifdef DEBUG_PRINT_FRAMES
        debugPrintFrame(*frame);
#endif
        if(frame->messageType() == "RECEIPT") {
            std::string receipt = frame->getHeader(ReceiptFrame::HEADER_RECEIPT_ID);
            if(hasReceipt(receipt)){
                Frame &receiptFrame = getFrameForReceipt(receipt);
                if(receiptFrame.messageType() == "SUBSCRIBE") {
                    std::string message = "Joined club " + receiptFrame.headers().at("destination");
                    _printer.println(message);
                } else if(receiptFrame.messageType() == "UNSUBSCRIBE") {
                    std::string message = "Exited club " + subscriptionIdToGenres.at(receiptFrame.getHeader(UnsubscribeFrame::HEADER_ID));
                    _printer.println(message);
                } else if(receiptFrame.messageType() == "DISCONNECT"){
                    _connection.close();
                    break;
                }
                removeReceipt(receipt);
            }
            else {
                _printer.println("received a receipt for an unknown frame.");
            }
        }
        else if(frame->messageType() == "MESSAGE"){
            std::string dest = frame->headers().at("destination");
            std::string body = frame->body();

            printMessage(dest, body);

            std::vector<std::string> message = Util::split(body, " ");

            if(message.size() > 4 && message[1] == "wish" && message[2] == "to" && message[3] == "borrow"){
                if(!handlerSomeoneWantToBorrow(dest, message)){ break; }
            } else if(message.size() > 2 && message[1] == "has" && message[2] != "added"){
                if(!handlerUserHasBook(dest, message)){ break; }
            } else if(message.size() > 3 && message[0] == "Taking" && message[message.size()-2] == "from"){
                handleSomeoneTakingBookFrom(dest, message);
            } else if(message.size() > 3 && message[0] == "Returning" && message[message.size()-2] == "to"){
                returnedBook(dest, message);
            } else if(message.size() == 2 && message[0] == "book" && message[1] == "status"){
                if(!sendBooksStatus(dest)){ break; }
            }
        }
        else if (frame->messageType() == ErrorFrame::MESSAGE_TYPE) {
            _connection.close();
            break;
        }
    }
}

void BookLibraryUser::returnedBook(const std::string &dest, const std::vector<std::string> &message) {
    std::string userOfTheBook = message[message.size()-1];
    if(userOfTheBook == _username){
        std::string bookName = message[1];
        for(unsigned long i=2; i < message.size()-2; i=i+1) {
            bookName.append(" ");
            bookName.append(message[i]);
        }
        _books.BookThatWasBorrowedHasReturn(dest, bookName);
    }
}

void BookLibraryUser::handleSomeoneTakingBookFrom(const std::string &dest, const std::vector<std::string> &message) {
    if(_username == message[message.size()-1]) {

        std::string bookName = message[1];
        for (unsigned long i = 2; i < message.size() - 2; i = i + 1) {
            bookName.append(" ");
            bookName.append(message[i]);
        }
        _books.borrowBookToUser(dest, bookName);
    }
}

bool BookLibraryUser::handlerUserHasBook(const std::string &dest, const std::vector<std::string> &message) {
    std::string userOfTheBook = message[0];
    if (userOfTheBook != _username) {
        std::string bookName = message[2];
        for (unsigned long i = 3; i < message.size(); i = i + 1) {
            bookName.append(" ");
            bookName.append(message[i]);
        }

        if (_books.wantToBorrow(dest, bookName)) {
            if (!sendTakingBookFrom(dest, bookName, userOfTheBook)) {
                return false;
            }
            _books.borrowBookFromUser(dest, bookName, userOfTheBook);
        }
    }

    return true;
}

bool BookLibraryUser::handlerSomeoneWantToBorrow(const std::string &dest, const std::vector<std::string> &message) {
    std::string name = message[0];
    if (name != _username) {
        // username want to borrow bookname
        std::string bookName = message[4];
        for (unsigned long i = 5; i < message.size(); i = i + 1) {
            bookName.append(" ");
            bookName.append(message[i]);
        }

        if (_books.hasBook(dest, bookName)) {
            return sendHasBookFrame(dest, bookName);

        }
    }

    return true;
}

bool BookLibraryUser::sendBooksStatus(const std::string &dest) {
    BookCollection bookCollection;

    std::string bodyMessage = _username + ":";
    if (_books.copyOfBookCollection(dest, bookCollection) && !bookCollection.isEmpty()) {
        auto book = bookCollection.begin();
        if (book->hasBook()) {
            bodyMessage.append(book->name());
        }
        for (++book; book != bookCollection.end(); ++book) {
            if (book->hasBook()) {
                bodyMessage.append(",");
                bodyMessage.append(book->name());
            }
        }
    }
    return sendSendFrame(dest, bodyMessage);
}

bool BookLibraryUser::sendHasBookFrame(const std::string &topic, const std::string &bookName) {
    std::string body = _username + " has " + bookName;
    return sendSendFrame(topic ,body);
}

bool BookLibraryUser::sendTakingBookFrom(const std::string &topic, const std::string &bookName, const std::string &from) {
    std::string body = "Taking " + bookName + " from " + from;
    return sendSendFrame(topic ,body);
}

void BookLibraryUser::printMessage(const std::string &topic, const std::string &body) {
    std::string message = topic + ":" + body;
    _printer.println(message);
}

bool BookLibraryUser::sendSendFrame(const std::string &topic, const std::string &body) {
    SendFrame frame = SendFrame(topic ,body);
    if(!_connection.sendFrame(frame)){
        _connection.close();
        return false;
    }
    return true;
}

#ifdef DEBUG_PRINT_FRAMES
void BookLibraryUser::debugPrintFrame(const Frame &frame) {
    std::unique_ptr<std::string> f = _connection.encode(frame);
    f->resize(f->length() + 1);
    _printer.println("----------\nreceived frame:\n" + *f);
}
#endif
