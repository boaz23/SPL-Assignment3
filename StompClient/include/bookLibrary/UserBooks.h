
#ifndef STOMPCLIENT_USERBOOKS_H
#define STOMPCLIENT_USERBOOKS_H

#include <unordered_map>
#include "BookCollection.h"

class UserBooks {
    // TODO: synchronize, only the map with a mutex

private:
    std::unordered_map<std::string, BookCollection> books;

public:
    void addBook(std::string genre, std::string book);
    void removeBook(std::string genre, std::string book);
    void addGenre(std::string genre);

    std::unordered_map<std::string, BookCollection>::iterator begin();
    std::unordered_map<std::string, BookCollection>::iterator end();

    std::unordered_map<std::string, BookCollection>::const_iterator cbegin();
    std::unordered_map<std::string, BookCollection>::const_iterator cend();
};


#endif //STOMPCLIENT_USERBOOKS_H
