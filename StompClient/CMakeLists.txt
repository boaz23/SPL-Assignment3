cmake_minimum_required(VERSION 3.15)
project(StompClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(StompClient src/main.cpp src/api/ConnectionHandler.cpp include/api/ConnectionHandler.h include/api/MessageEncoderDecoder.h src/Array.cpp include/Array.h src/stomp/Frame.cpp include/stomp/Frame.h src/stomp/StompMessageEncoderDecoder.cpp include/stomp/StompMessageEncoderDecoder.h src/bookLibrary/UserBooks.cpp include/bookLibrary/UserBooks.h src/bookLibrary/BookCollection.cpp include/bookLibrary/BookCollection.h src/bookLibrary/Main.cpp include/bookLibrary/Main.h include/Printer.h src/bookLibrary/BookLibraryUser.cpp include/bookLibrary/BookLibraryUser.h src/stomp/StompConnectionHandler.cpp include/stomp/StompConnectionHandler.h include/stomp/StompVersion.h src/Util.cpp include/Util.h include/bookLibrary/Book.h)
target_link_libraries(StompClient ${Boost_LIBRARIES})