
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
    void removeBook(const std::string &book);

    std::vector<Book>::iterator begin();
    std::vector<Book>::iterator end();

    std::vector<Book>::const_iterator cbegin();
    std::vector<Book>::const_iterator cend();
};


#endif //STOMPCLIENT_BOOKCOLLECTION_H
