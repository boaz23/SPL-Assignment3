#ifndef STOMPCLIENT_BOOK_H
#define STOMPCLIENT_BOOK_H

#include <string>

enum BookState {
    HAVE,
    BORROWED_TO,
    WANT_TO_BORROW,
    BORROWED_FROM,

    // When we borrowed a book from someone and then we lent it to someone (maybe the same guy)
    BORROWED_FROM_AND_BORROWED_TO,
};

class Book{
private:
    std::string _name;

    BookState _bookState;
    //if the book is borrowed the bool will be true and the string
    // will contain the username of the user that borrowed the book
    std::string _borrowedFrom;
    std::string _emptyStr;

    Book(std::string bookName, BookState bookState, std::string borrowedFrom);
    void setState(BookState bookState);

public:
    Book();
    static Book newBook(const std::string &bookName);
    static Book wantToBorrowBook(const std::string &bookName);
    static Book borrowBookTo(const std::string &bookName);
    static Book borrowedBookFrom(const std::string &bookName, const std::string &from);

    const std::string& name() const;
    std::string& getUsernameOfBookThatWasBorrowedFrom();

    bool hasBook() const;
    bool wantToBorrow() const;
    bool isBorrowedFromSomeone() const;
    bool isBorrowedFrom(const std::string &from) const;
    bool isBorrowedTo() const;
    void markAsBorrowed(const std::string &from);

    void BorrowedBookFromUserHasReturned();
    void borrowTheBookToSomeone();
};

#endif //STOMPCLIENT_BOOK_H
