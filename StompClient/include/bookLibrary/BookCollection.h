
#ifndef STOMPCLIENT_BOOKCOLLECTION_H
#define STOMPCLIENT_BOOKCOLLECTION_H

#include <unordered_set>

class BookCollection {
    // TODO: not need to synchronize

private:
    const std::unordered_set<std::string> books;

public:
    void addBook(const std::string &book);
    void removeBook(const std::string &book);

    std::unordered_set<std::string>::iterator begin();
    std::unordered_set<std::string>::iterator end();

    std::unordered_set<std::string>::const_iterator cbegin();
    std::unordered_set<std::string>::const_iterator cend();
};


#endif //STOMPCLIENT_BOOKCOLLECTION_H
