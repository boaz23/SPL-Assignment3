
#include "../../include/bookLibrary/BookCollection.h"

BookCollection::BookCollection(): _books(10) {

}

void BookCollection::addBook(const std::string &bookName) {
    Book book = Book::newBook(bookName);
    addBook(book);
}

void BookCollection::addBook(const Book& book) {
    //TODO check if the vector copy the Book or just holding the reference
    _books.push_back(book);
}

void BookCollection::removeBook(const std::string &bookName) {
    //TODO: if there is multiply book check which one to remove
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
            book.setToUserOwnership();
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
    //TODO: check if a borrowed book also can be borrowed to user, if so include it
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
bool BookCollection::isBorrowedFrom(const std::string &bookName, const std::string &from) const {
    for(const auto& book : _books){
        if(book.name() == bookName && book.isBorrowedFrom(from)){
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
}

/**
 * Remove one instance of a borrowed book
 * Return the number of book with the same name that remain in the vector
 * @param bookName
 * @param from
 * @return
 */
int BookCollection::removeBorrowedBook(const std::string &bookName, const std::string &from) {
    int numberOfBooks = -1;
    unsigned long index = 0;
    bool b = false;
    for(auto& book : _books){
        if(book.name() == bookName){
            numberOfBooks = numberOfBooks + 1;
            if(book.isBorrowedFrom(from) && !b) {
                _books.erase(_books.begin()+index);
                b = true;
            }
        }
        index = index + 1;
    }
    return numberOfBooks;
}
