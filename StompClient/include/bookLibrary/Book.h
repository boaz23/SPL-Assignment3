#ifndef STOMPCLIENT_BOOK_H
#define STOMPCLIENT_BOOK_H

#include <string>

class Book{
private:
    std::string _book;
    std::string _topic;

    bool wantToBoorow;
    //if the book is borrowed the bool will be true and the string
    // will contain the username of the user that borrowed the book
    std::pair<bool, std::string> _borrowedTo;

public:

    enum BookState {HAVE, BORROWED, WANT_TO_BORROW, BORROWED_FROM};

    static Book newBook(const std::string &genre, const std::string &bookName);
    static Book wantToBorrowBook(const std::string &genre, const std::string &bookName);
    static Book borrowedBookTo(const std::string &genre, const std::string &bookName, const std::string &to);
    static Book borrowedBookFrom(const std::string &genre, const std::string &bookName, const std::string &from);

    const std::string& bookName() const;
    //if the book is not borrowed
    bool hasBook() const;
    //the book is in the want to borrow list
    bool wantToBorrow() const;

    bool isBorrowed() const;
};

#endif //STOMPCLIENT_BOOK_H
