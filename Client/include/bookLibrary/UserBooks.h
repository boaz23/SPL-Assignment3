#ifndef STOMPCLIENT_USERBOOKS_H
#define STOMPCLIENT_USERBOOKS_H

#include <unordered_map>
#include <mutex>
#include "BookCollection.h"

class UserBooks {
private:
    std::unordered_map<std::string, BookCollection> _books;
    std::unordered_map<std::string, std::string> _bookToGenreMap;

    std::mutex _mutex;

    void addBookToBookCollection(const std::string &genre, const std::string &book);
    void addBookToBookCollection(const std::string &genre, const Book &book);
    void addBook(const std::string &genre, const Book& book);

    bool isBookExistWithSameTopic(const std::string &genre, const std::string &bookName) const;

public:
    UserBooks();
    UserBooks(const UserBooks &other);
    UserBooks& operator=(const UserBooks &other);

    /**
     * Clear all books.
     */
    void clear();

    /**
     * Set the @param genre according to the @param bookName
     * @param bookName
     * @param genre
     * @return True if the @param genre was set, false otherwise
     */
    bool getBookGenre(const std::string &bookName, std::string &genre);

    /**
     * Add a book with @param book the name of the book and @param genre the genre of it.
     * @param genre
     * @param book
     */
    void addBook(const std::string &genre, const std::string &book);

    /**
     * Check if the @param bookName with the @param genre is in the users position
     * @param genre
     * @param bookName
     * @return Return true if the user has the book.
     */
    bool hasBook(const std::string &genre, const std::string &bookName);

    /**
     * Check if the user want to borrow the @param bookName with the @param genre
     * @param genre
     * @param bookName
     * @return true if the user marked it as want to borrow, false otherwise
     */
    bool wantToBorrow(const std::string &genre, const std::string &bookName);

    /**
     * Add a book to the user collection in want to borrow state
     * @param genre
     * @param bookName
     */
    void addBookAsWantToBorrow(const std::string &genre, const std::string &bookName);
    
    /**
     * Add @param bookName with the @param genre to the collection as borrowed from @param from
     * @param genre
     * @param bookName
     * @param from
     */
    void borrowBookFromUser(const std::string &genre, const std::string &bookName, const std::string &from);

    /**
     * Set the @param bookName with the @param genre as borrowed to user
     * @param genre
     * @param bookName
     */
    void borrowBookToUser(const std::string &genre, const std::string &bookName);

    /**
     * Search of the first appearance of @param bookName with the @param genre that was borrowed to someone
     * @param genre
     * @param bookName
     */
    void BookThatWasBorrowedHasReturn(const std::string &genre, const std::string &bookName);

    /**
     * Set @param borrowedFrom with the username, if exists, which the @param bookName with the @param genre
     * was borrowed to
     * @param genre
     * @param bookName
     * @param borrowedFrom
     * @return True if the @param borrowedFrom was filled with data, false otherwise
     */
    bool getBorrowedFromUsername(const std::string &genre, const std::string &bookName, std::string &borrowedFrom);

    /**
     * Remove the first appearance of the @param bookName with the @param genre that was borrowed from someone
     * @param genre
     * @param bookName
     */
    void removeBorrowedBook(const std::string &genre, const std::string &bookName);

    /**
     * set @param bookCollection with a copy of the user book collection of @param genre
     * @param genre
     * @param bookCollection
     * @return True if @param bookCollection was set, false otherwise
     */
    bool copyOfBookCollection(const std::string &genre, BookCollection &bookCollection);

};


#endif //STOMPCLIENT_USERBOOKS_H
