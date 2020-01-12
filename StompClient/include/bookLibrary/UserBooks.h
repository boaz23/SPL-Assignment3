#ifndef STOMPCLIENT_USERBOOKS_H
#define STOMPCLIENT_USERBOOKS_H

#include <unordered_map>
#include "BookCollection.h"

class UserBooks {
    // TODO: synchronize, only the map with a mutex
    // TODO: delete the bookCollection maybe rule of five
private:
    std::unordered_map<std::string, BookCollection> _books;
    std::unordered_map<std::string, std::string> _bookToGenreMap;

    void addBookToBookCollection(const std::string &genre, const std::string &book);
    void addBookToBookCollection(const std::string &genre, const Book &book);
    void addBook(const std::string &genre, const Book& book);

public:
    UserBooks();

    void clear();

    bool getBookGenre(const std::string &bookName, std::string &genre) const;

    void addBook(const std::string &genre, const std::string &book);
    void removeBook(const std::string &genre, const std::string &book);

    bool hasBook(const std::string &genre, const std::string &bookName) const;
    // retrurn if the user want to borrow the book
    bool wantToBorrow(const std::string &genre, const std::string &bookName) const;

    void addBookAsWantToBorrow(const std::string &genre, const std::string &bookName);

    bool isBorrowedFrom(const std::string &genre, const std::string &bookName, const std::string &from) const;
    //add the book to the borrowed books
    void borrowBookFromUser(const std::string &genre, const std::string &bookName, const std::string &from);

    void borrowBookToUser(const std::string &genre, const std::string &bookName);
    //remove the book from the borrowed book list
    void BookThatWasBorrowedHasReturn(const std::string &genre, const std::string &bookName);

    bool getBorrowedFromUsername(const std::string &genre, const std::string &bookName, const std::string &borrowedFrom);

    void removeBorrowedBook(const std::string &genre, const std::string &bookName);

    // The book collection can be modified so a copy of it in the current state should be returned
    bool copyOfBookCollection(const std::string &genre, BookCollection &bookCollection);
};


#endif //STOMPCLIENT_USERBOOKS_H
