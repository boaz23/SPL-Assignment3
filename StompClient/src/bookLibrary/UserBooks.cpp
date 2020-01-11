
#include "../../include/bookLibrary/UserBooks.h"

UserBooks::UserBooks() : _books(), _bookToGenreMap() {
}

void UserBooks::addBook(const std::string &genre, const std::string &book) {
    if(_bookToGenreMap.count(book) == 0){
       _bookToGenreMap.insert(std::make_pair(book, genre));
        addBookToBookCollection(genre, book);

    } else {
        //TODO: may throw exeption here
    }
}

void UserBooks::addBookToBookCollection(const std::string &genre, const Book& book) {
    if(_books.count(genre) > 0){
        BookCollection bookCollection1 = _books.at(genre);
        bookCollection1.addBook(book);
    } else {
        auto bookCollection1 = BookCollection();
        bookCollection1.addBook(book);
        _books.insert(std::make_pair(genre, bookCollection1));
    }
}

void UserBooks::addBookToBookCollection(const std::string &genre, const std::string &book) {
    if(_books.count(genre) > 0){
        BookCollection bookCollection1 = _books.at(genre);
        bookCollection1.addBook(book);
    } else {
        auto bookCollection1 = BookCollection();
        bookCollection1.addBook(book);
        _books.insert(std::make_pair(genre, bookCollection1));
    }
}

void UserBooks::removeBook(const std::string &genre, const std::string &book) {
    if(_bookToGenreMap.count(book) > 0) {
        _books.erase(book);
        if(_books.count(genre) > 0){
            BookCollection bookCollection1 = _books.at(genre);
            bookCollection1.removeBook(book);
        }
    }
}

/**
 * The user have the book, not borrowed
 * @param genre
 * @param bookName
 * @return
 */
bool UserBooks::hasBook(const std::string &genre, const std::string &bookName) const{
    if(_bookToGenreMap.count(bookName) > 0 && _bookToGenreMap.at(bookName) == genre){
        BookCollection bookCollection1 = _books.at(genre);
        const Book &book = bookCollection1.getBook(bookName);
        if(book.hasBook()){
            return true;
        }
    }
    return false;
}

/**
 * The user want to borrow this book.
 * @param genre
 * @param bookName
 * @return
 */
bool UserBooks::wantToBorrow(const std::string &genre, const std::string &bookName) const {
    if(_bookToGenreMap.count(bookName) == 0){
        BookCollection bookCollection1 = _books.at(genre);
        if(bookCollection1.wantToBorrowBook(bookName)){
            return true;
        }
    }
    return false;
}

/**
 * Create a new a book and add it to the corresponding topic as want to borrow
 * @param genre
 * @param bookName
 */
void UserBooks::addBookAsWantToBorrow(const std::string &genre, const std::string &bookName) {
    Book book = Book(genre, bookName, Book::WANT_TO_BORROW);
    addBookToBookCollection(genre, book);
}



