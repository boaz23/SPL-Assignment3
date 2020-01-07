
#include "../../include/bookLibrary/BookLibraryUser.h"

BookLibraryUser::BookLibraryUser(std::string username, StompConnectionHandler &connection,
                                 StompMessageEncoderDecoder &encdec) : username(username), _connection(connection), _encdec(encdec) {}
