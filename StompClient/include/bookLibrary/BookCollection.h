
#ifndef STOMPCLIENT_BOOKCOLLECTION_H
#define STOMPCLIENT_BOOKCOLLECTION_H

#include <unordered_set>
#include <vector>
#include "Book.h"

class BookCollection {
    // TODO: not need to synchronize

private:
    const std::vector<Book> books;

public:
    void addBook(const std::string &book);
    void addBook(const Book book);
    void removeBook(const std::string &book);

    void userReturnTheBook(const std::string &book, const std::string &username);

    bool isEmpty() const;
    bool hasBook(const std::string &book) const;
    bool isBorrowed(const std::string &book) const;
    bool wantToBorrowBook(const std::string &book) const;

    int removeBorrowedBook(const std::string &book);

    std::vector<Book>::iterator begin();
    std::vector<Book>::iterator end();

    std::vector<Book>::const_iterator begin() const;
    std::vector<Book>::const_iterator end() const;
};


#endif //STOMPCLIENT_BOOKCOLLECTION_H
