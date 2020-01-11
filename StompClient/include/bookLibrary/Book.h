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

    enum BookState {HAVE, BORROWED, WANT_TO_BORROW};

    Book(const std::string &genre, const std::string &bookName, BookState bookState);

    const std::string& bookName() const;
    //if the book is not borrowed
    bool hasBook() const;
    //the book is in the want to borrow list
    bool wantToBorrow() const;
};

#endif //STOMPCLIENT_BOOK_H
