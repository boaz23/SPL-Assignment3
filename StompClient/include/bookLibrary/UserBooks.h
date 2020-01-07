#ifndef STOMPCLIENT_USERBOOKS_H
#define STOMPCLIENT_USERBOOKS_H

#include <unordered_map>
#include "BookCollection.h"

class UserBooks {
    // TODO: synchronize, only the map with a mutex

private:
    const std::unordered_map<std::string, BookCollection> books;

public:
    UserBooks();

    void addBook(const std::string &genre, const std::string &book);
    void removeBook(const std::string &genre, const std::string &book);
    void addGenre(const std::string &genre);

    std::unordered_map<std::string, BookCollection>::iterator begin();
    std::unordered_map<std::string, BookCollection>::iterator end();

    std::unordered_map<std::string, BookCollection>::const_iterator cbegin();
    std::unordered_map<std::string, BookCollection>::const_iterator cend();
};


#endif //STOMPCLIENT_USERBOOKS_H
