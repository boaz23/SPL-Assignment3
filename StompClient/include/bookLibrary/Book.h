#ifndef STOMPCLIENT_BOOK_H
#define STOMPCLIENT_BOOK_H

#include <string>

class Book{
private:
    std::string _book;

    bool wantToBoorow;
    //if the book is borrowed the bool will be true and the string
    // will contain the username of the user that borrowed the book
    std::pair<bool, std::string> _borrowedTo;

public:
    //TODO: consider adding a std::string topic to this class
    enum BookState {HAVE, BORROWED, WANT_TO_BORROW, BORROWED_FROM};

    static Book newBook(const std::string &bookName);
    static Book wantToBorrowBook(const std::string &bookName);
    static Book borrowedBookTo(const std::string &bookName, const std::string &to);
    static Book borrowedBookFrom(const std::string &bookName, const std::string &from);

    const std::string& name() const;
    //if the book is not borrowed
    bool hasBook() const;
    //the book is in the want to borrow list
    bool wantToBorrow() const;
    //Borrowed from a user
    bool isBorrowed() const;
    //Check if Borrowed to a user
    bool isBorrowedTo(const std::string &username) const;

    void setToUserOwnership();
};

#endif //STOMPCLIENT_BOOK_H
