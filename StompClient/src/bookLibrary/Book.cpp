//
// Created by netanel on 12/01/2020.
//

#include "../../include/bookLibrary/Book.h"

#include <utility>

Book::Book(std::string bookName, BookState bookState,
        std::string borrowedFrom):
        _book(std::move(bookName)), _bookState(bookState),
        _borrowedFrom(std::move(borrowedFrom)) {
}

Book Book::newBook(const std::string &bookName){
    Book book = Book(bookName, BookState::HAVE, "");
    return book;
}

Book Book::wantToBorrowBook(const std::string &bookName) {
    Book book = Book(bookName, BookState::WANT_TO_BORROW, "");
    return book;
}

Book Book::borrowedBookTo(const std::string &bookName) {
    Book book = Book(bookName, BookState::BORROWED, "");
    return book;
}

Book Book::borrowedBookFrom(const std::string &bookName, const std::string &from) {
    Book book = Book(bookName, BookState::BORROWED_FROM, from);
    return book;
}

void Book::setState(BookState bookState) {
    _bookState = bookState;
}

const std::string& Book::name() const {
    return _book;
}

bool Book::hasBook() const {
    return _bookState == BookState::HAVE;
}

bool Book::wantToBorrow() const {
    return _bookState == BookState::WANT_TO_BORROW;
}

bool Book::isBorrowedFromSomeone() const {
    return _bookState == BookState::BORROWED_FROM;
}

bool Book::isBorrowedFrom(const std::string &from) const {
    if(_bookState == BookState::BORROWED_FROM){
        if(_borrowedFrom == from){
            return true;
        }
    }
    return false;
}

bool Book::isBorrowedTo() const {
    return _bookState == BookState::BORROWED;
}

void Book::setToUserOwnership() {
    _bookState = BookState::HAVE;
    _borrowedFrom = "";
}


