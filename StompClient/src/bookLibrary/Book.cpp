//
// Created by netanel on 12/01/2020.
//

#include "../../include/bookLibrary/Book.h"

#include <utility>
#include <stdexcept>

Book::Book() : _name(), _bookState(), _borrowedFrom(), _emptyStr() { }

Book::Book(std::string bookName, BookState bookState,
    std::string borrowedFrom):
    _name(std::move(bookName)), _bookState(bookState),
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

Book Book::borrowBookTo(const std::string &bookName) {
    Book book = Book(bookName, BookState::BORROWED_TO, "");
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
    return _name;
}

bool Book::hasBook() const {
    return _bookState == BookState::HAVE || _bookState == BookState::BORROWED_FROM;
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
    return _bookState == BookState::BORROWED_TO || _bookState == BookState::BORROWED_FROM_AND_BORROWED_TO;
}

void Book::BorrowedBookFromUserHasReturned() {
    if(_bookState == BookState::BORROWED_TO){
        _bookState = BookState::HAVE;
        _borrowedFrom = "";
    } else if(_bookState == BookState::BORROWED_FROM_AND_BORROWED_TO){
        _bookState = BookState::BORROWED_FROM;
    }
}

void Book::borrowTheBookToSomeone() {
    if(_bookState == BookState::HAVE){
        _bookState = BookState::BORROWED_TO;
    } else if(_bookState == BookState::BORROWED_FROM){
        _bookState = BookState::BORROWED_FROM_AND_BORROWED_TO;
    } else {
        throw std::runtime_error("Cant borrow the book in this state");
    }
}

std::string& Book::getUsernameOfBookThatWasBorrowedFrom() {
    if(_bookState == BookState::BORROWED_FROM){
        return _borrowedFrom;
    } else {
        return _emptyStr;
    }
}


