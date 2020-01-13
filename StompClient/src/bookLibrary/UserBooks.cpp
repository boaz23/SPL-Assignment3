
#include "../../include/bookLibrary/UserBooks.h"
#include "../../include/Lock.h"

UserBooks::UserBooks() : _books(), _bookToGenreMap(), _mutex() {
}

void UserBooks::addBook(const std::string &genre, const std::string &book) {
    mutex_lock lock(_mutex);

    if(_bookToGenreMap.count(book) == 0){
       _bookToGenreMap.insert(std::make_pair(book, genre));
        addBookToBookCollection(genre, book);
    } else if(_bookToGenreMap.at(book) == genre) {
        addBookToBookCollection(genre, book);
    } else {
        throw std::runtime_error("Book cant have two different topics");
    }
}

void UserBooks::addBook(const std::string &genre, const Book& book) {
    mutex_lock lock(_mutex);

    if (_bookToGenreMap.count(book.name()) > 0) {
        if(_bookToGenreMap.at(book.name()) == genre) {
            addBookToBookCollection(genre, book);
        } else {
            throw std::runtime_error("Book cant have two different topics");
        }
    } else {
        _bookToGenreMap.insert(std::make_pair(book.name(), genre));
        addBookToBookCollection(genre, book);
    }
}

void UserBooks::addBookToBookCollection(const std::string &genre, const Book &book) {
    if (_books.count(genre) > 0) {
        BookCollection &bookCollection1 = _books.at(genre);
        bookCollection1.addBook(book);
    } else {
        auto bookCollection1 = BookCollection();
        bookCollection1.addBook(book);
        _books.insert(std::make_pair(genre, bookCollection1));
    }
}

void UserBooks::addBookToBookCollection(const std::string &genre, const std::string &book) {
    if(_books.count(genre) > 0){
        BookCollection &bookCollection1 = _books.at(genre);
        bookCollection1.addBook(book);
    } else {
        auto bookCollection1 = BookCollection();
        bookCollection1.addBook(book);
        _books.insert(std::make_pair(genre, bookCollection1));
    }
}

/**
 * The user have the book, not borrowed
 * @param genre
 * @param bookName
 * @return
 */
bool UserBooks::hasBook(const std::string &genre, const std::string &bookName) {
    mutex_lock lock(_mutex);

    if(isBookExistWithSameTopic(genre, bookName)){
        BookCollection &bookCollection1 = _books.at(genre);
        if(bookCollection1.hasBook(bookName)){
            return true;
        }
    }
    return false;
}

bool UserBooks::isBookExistWithSameTopic(const std::string &genre,
                                         const std::string &bookName) const { return _bookToGenreMap.count(bookName) > 0 && _bookToGenreMap.at(bookName) == genre; }

/**
 * The user want to borrow this book.
 * @param genre
 * @param bookName
 * @return
 */
bool UserBooks::wantToBorrow(const std::string &genre, const std::string &bookName) {
    mutex_lock lock(_mutex);

    if(_bookToGenreMap.count(bookName)){
        BookCollection &bookCollection1 = _books.at(genre);
        return bookCollection1.wantToBorrowBook(bookName);
    }
    return false;
}

/**
 * Create a new a book and add it to the corresponding topic as want to borrow
 * @param genre
 * @param bookName
 */
void UserBooks::addBookAsWantToBorrow(const std::string &genre, const std::string &bookName) {
    Book book = Book::wantToBorrowBook(bookName);
    addBook(genre, book);
}

void UserBooks::borrowBookFromUser(const std::string &genre, const std::string &bookName, const std::string &from) {
    BookCollection &bookCollection = _books[genre];
    bookCollection.markAsBorrowed(bookName, from);
}

void UserBooks::removeBorrowedBook(const std::string &genre, const std::string &bookName) {
    mutex_lock lock(_mutex);

    if(isBookExistWithSameTopic(genre, bookName)) {
        BookCollection &bookCollection1 = _books.at(genre);
        //There is no more instances of this book in the collection
        if(bookCollection1.removeBorrowedBook(bookName) == 0){
            _bookToGenreMap.erase(bookName);
        }
    }
}

/**
 * When Returning message is recieved, a different user return a book the was borrow from the current, so the state
 * of the book in the collection is back to be at the current user ownership.
 * @param genre
 * @param bookName
 * @param returnedFromUser
 */
void UserBooks::BookThatWasBorrowedHasReturn(const std::string &genre, const std::string &bookName) {
    mutex_lock lock(_mutex);

    if(isBookExistWithSameTopic(genre, bookName)) {
        BookCollection &bookCollection1 = _books.at(genre);
        bookCollection1.userReturnTheBook(bookName);
    }
}

bool UserBooks::copyOfBookCollection(const std::string &genre, BookCollection &bookCollection) {
    mutex_lock lock(_mutex);

    if (_books.count(genre) > 0) {
        bookCollection = _books.at(genre);
        return true;
    }

    return false;
}

void UserBooks::clear() {
    mutex_lock lock(_mutex);

    _books.clear();
    _bookToGenreMap.clear();
}

bool UserBooks::getBorrowedFromUsername(const std::string &genre, const std::string &bookName, std::string &borrowedFrom) {
    mutex_lock lock(_mutex);

    if(isBookExistWithSameTopic(genre, bookName)) {
        BookCollection &bookCollection1 = _books.at(genre);
        return bookCollection1.getBorrowedFrom(bookName, borrowedFrom);
    }

    return false;
}

bool UserBooks::getBookGenre(const std::string &bookName, std::string &genre) {
    mutex_lock lock(_mutex);

    if(_bookToGenreMap.count(bookName) > 0){
        genre = _bookToGenreMap.at(bookName);
        return true;
    }
    return false;
}

void UserBooks::borrowBookToUser(const std::string &genre, const std::string &bookName) {
    mutex_lock lock(_mutex);

    if(_books.count(genre) > 0) {
        BookCollection &bookCollection1 = _books.at(genre);
        bookCollection1.borrowBookToUser(bookName);
    } else {
        throw std::runtime_error("Someone was taking book from us because we said we have it...");
    }
}

