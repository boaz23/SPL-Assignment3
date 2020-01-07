cmake_minimum_required(VERSION 3.15)
project(StompClient)

set(CMAKE_CXX_STANDARD 11)

add_executable(StompClient src/main.cpp src/api/ConnectionHandler.cpp include/api/ConnectionHandler.h src/api/Protocol.cpp include/api/Protocol.h src/api/MessageEncoderDecoder.cpp include/api/MessageEncoderDecoder.h src/Array.cpp include/Array.h src/stomp/StompProtocol.cpp include/stomp/StompProtocol.h src/stomp/Frame.cpp include/stomp/Frame.h src/stomp/StompMessageEncoderDecoder.cpp include/stomp/StompMessageEncoderDecoder.h src/bookLibrary/UserBooks.cpp include/bookLibrary/UserBooks.h src/bookLibrary/BookCollection.cpp include/bookLibrary/BookCollection.h src/bookLibrary/BookLibraryUser.cpp include/bookLibrary/BookLibraryUser.h)