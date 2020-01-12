
#ifndef STOMPCLIENT_BOOKCOLLECTION_H
#define STOMPCLIENT_BOOKCOLLECTION_H

#include <unordered_set>
#include <vector>
#include "Book.h"

class BookCollection {
private:
    std::vector<Book> _books;

public:

    BookCollection();

    void addBook(const std::string &bookName);
    void addBook(const Book& book);

    void userReturnTheBook(const std::string &bookName);

    bool isEmpty() const;
    bool hasBook(const std::string &bookName) const;
    bool isBorrowedFrom(const std::string &bookName, const std::string &from) const;
    bool wantToBorrowBook(const std::string &bookName) const;

    int removeBorrowedBook(const std::string &bookName);

    std::vector<Book>::iterator begin();
    std::vector<Book>::iterator end();

    std::vector<Book>::const_iterator begin() const;
    std::vector<Book>::const_iterator end() const;
};


#endif //STOMPCLIENT_BOOKCOLLECTION_H
