#ifndef STOMPCLIENT_USERBOOKS_H
#define STOMPCLIENT_USERBOOKS_H

#include <unordered_map>
#include "BookCollection.h"

class UserBooks {
    // TODO: synchronize, only the map with a mutex

private:
    const std::unordered_map<std::string, BookCollection> _books;
    const std::unordered_map<std::string, std::string> _bookToGenreMap;

public:
    UserBooks();

    void clear();

    bool getBookGenre(std::string bookName, std::string &genre);

    void addBook(const std::string &genre, const std::string &book);
    void removeBook(const std::string &genre, const std::string &book);

    bool hasBook(const std::string &genre, const std::string &bookName);
    // retrurn if the user want to borrow the book
    bool wantToBorrow(const std::string &genre, const std::string &bookName);
    bool isBorrowed(const std::string &genre, const std::string &bookName);
    //add the book to the borrowed books
    void borrowBook(const std::string &genre, const std::string &bookName, const std::string &from);
    //remove the book from the borrowed book list
    void returnBorrowedBook(const std::string &genre, const std::string &bookName);

    bool getBorrowedFromUsername(const std::string &genre, const std::string &bookName, std::string &borrowedFrom);
    void removeBorrowedBook(const std::string &genre, const std::string &bookName);

    const BookCollection& bookCollection(const std::string &genre);

    //TODO: consider removing this
    std::unordered_map<std::string, BookCollection>::iterator begin();
    std::unordered_map<std::string, BookCollection>::iterator end();

    std::unordered_map<std::string, BookCollection>::const_iterator cbegin();
    std::unordered_map<std::string, BookCollection>::const_iterator cend();
};


#endif //STOMPCLIENT_USERBOOKS_H
