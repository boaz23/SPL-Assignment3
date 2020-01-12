#ifndef STOMPCLIENT_BOOK_H
#define STOMPCLIENT_BOOK_H

#include <string>

enum BookState {HAVE, BORROWED, WANT_TO_BORROW, BORROWED_FROM};

class Book{
private:
    std::string _book;

    BookState _bookState;
    bool _wantToBoorow;
    //if the book is borrowed the bool will be true and the string
    // will contain the username of the user that borrowed the book
    std::string _borrowedFrom;

    Book(std::string bookName, BookState bookState, bool wantToBorrow, std::string borrowedFrom);
    void setState(BookState bookState);

public:
    //TODO: consider adding a std::string topic to this class

    static Book newBook(const std::string &bookName);
    static Book wantToBorrowBook(const std::string &bookName);
    static Book borrowedBookTo(const std::string &bookName);
    static Book borrowedBookFrom(const std::string &bookName, const std::string &from);

    const std::string& name() const;
    bool hasBook() const;
    bool wantToBorrow() const;
    bool isBorrowedFrom(const std::string &from) const;
    bool isBorrowedTo() const;

    void setToUserOwnership();
};

#endif //STOMPCLIENT_BOOK_H
