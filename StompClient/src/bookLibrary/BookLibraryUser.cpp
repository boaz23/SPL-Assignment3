
#include "../../include/bookLibrary/BookLibraryUser.h"

BookLibraryUser::BookLibraryUser(std::string username, std::string password, StompConnectionHandler &connection,
                                 StompMessageEncoderDecoder &encdec) : username(username), password(password), _connection(connection), _encdec(encdec),
                                 books(), receipts(), pendingBorrows(), sucessfulBorrows() {}
