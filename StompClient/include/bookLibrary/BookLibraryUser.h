
#ifndef STOMPCLIENT_BOOKLIBRARYUSER_H
#define STOMPCLIENT_BOOKLIBRARYUSER_H

#include "UserBooks.h"

class BookLibraryUser {
private:
    UserBooks _books;
    std::string _username;

public:
    BookLibraryUser(std::string username);

    UserBooks& books();
};


#endif //STOMPCLIENT_BOOKLIBRARYUSER_H
