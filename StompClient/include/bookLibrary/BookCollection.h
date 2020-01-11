
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
    bool isEmpty() const;

    const Book& getBook(const std::string &book) const;

    bool wantToBorrowBook(const std::string &book) const;

    std::vector<Book>::iterator begin();
    std::vector<Book>::iterator end();

    std::vector<Book>::const_iterator begin() const;
    std::vector<Book>::const_iterator end() const;
};


#endif //STOMPCLIENT_BOOKCOLLECTION_H
