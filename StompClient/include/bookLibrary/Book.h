#ifndef STOMPCLIENT_BOOK_H
#define STOMPCLIENT_BOOK_H

#include <string>

class Book{
private:
    std::string book;

public:
    const std::string& bookName() const;
};

#endif //STOMPCLIENT_BOOK_H
