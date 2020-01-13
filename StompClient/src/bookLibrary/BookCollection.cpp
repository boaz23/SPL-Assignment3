
#include "../../include/bookLibrary/BookCollection.h"

BookCollection::BookCollection(): _books() {

}

void BookCollection::addBook(const std::string &bookName) {
    Book book = Book::newBook(bookName);
    addBook(book);
}

void BookCollection::addBook(const Book& book) {
    //TODO check if the vector copy the Book or just holding the reference
    _books.push_back(book);
}

/**
 * Search the collection for this book. if there is a book that his state is Borrowed_To_User
 * from the @username set his state back to the ownership of this user.
 * @param bookName
 * @param username
 */
void BookCollection::userReturnTheBook(const std::string &bookName) {
    for(auto book : _books){
        if(book.isBorrowedTo()){
            book.BorrowedBookFromUserHasReturned();
            break;
        }
    }
}

bool BookCollection::isEmpty() const {
    return _books.empty();
}

/**
 * Return true if there is an instance of the book in the user ownership
 * @param bookName
 * @return
 */
bool BookCollection::hasBook(const std::string &bookName) const {
    for(const auto& book : _books){
        if(book.name() == bookName && book.hasBook()){
            return true;
        }
    }
    return false;
}

/**
 * Return true if there is an instance of the book in borrowed state
 * @param bookName
 * @param from
 * @return
 */
bool BookCollection::getBorrowedFrom(const std::string &bookName, std::string &from){
    for(auto& book : _books){
        if(book.name() == bookName && book.isBorrowedFromSomeone()){
            from = book.getUsernameOfBookThatWasBorrowedFrom();
            return true;
        }
    }
    return false;
}

/**
 * Return true if there is an instance of the book in want to borrowed state
 * @param bookName
 * @return
 */
bool BookCollection::wantToBorrowBook(const std::string &bookName) const {
    for(const auto& book : _books){
        if(book.name() == bookName && book.wantToBorrow()){
            return true;
        }
    }

    return false;
}

void BookCollection::markAsBorrowed(const std::string &bookName, const std::string &from) {
    for(auto& book : _books){
        if(book.name() == bookName && book.wantToBorrow()){
            book.markAsBorrowed(from);
            break;
        }
    }
}

/**
 * Remove one instance of a borrowed book
 * Return the number of book with the same name that remain in the vector
 * @param bookName
 * @param from
 * @return
 */
int BookCollection::removeBorrowedBook(const std::string &bookName) {
    int numberOfBooks = -1;
    unsigned long i = 0;
    bool b = false;
    for(auto& book : _books){
        if(book.name() == bookName){
            numberOfBooks = numberOfBooks + 1;
            if (book.isBorrowedFromSomeone() && !b) {
                _books.erase(_books.begin() + i);
                b = true;
            }
        }
        i = i + 1;
    }
    return numberOfBooks;
}

std::vector<Book>::iterator BookCollection::begin() {
    return _books.begin();
}

std::vector<Book>::iterator BookCollection::end() {
    return _books.end();
}

std::vector<Book>::const_iterator BookCollection::begin() const {
    return _books.begin();
}

std::vector<Book>::const_iterator BookCollection::end() const {
    return _books.end();
}

bool BookCollection::borrowBookToUser(const std::string &bookName) {
    for(auto& book : _books){
        if(book.name() == bookName && book.hasBook()){
            book.borrowTheBookToSomeone();
            return true;
        }
    }
    return false;
}
