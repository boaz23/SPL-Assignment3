CC = g++
CFLAGS = -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS = -lboost_system -pthread

link: StompBookClubClient
	@echo 'Linking...'
	$(CC) -o bin/StompBookClubClient bin/main.o bin/api-ConnectionHandler.o bin/bookLibrary-Book.o bin/bookLibrary-BookCollection.o bin/bookLibrary-BookLibraryUser.o bin/bookLibrary-Main.o bin/bookLibrary-UserBooks.o bin/stomp-Frame.o bin/stomp-StompConnectionHandler.o bin/stomp-StompMessageEncoderDecoder.o bin/Printer.o bin/Util.o $(LDFLAGS)

StompBookClubClient: bin/main.o bin/api-ConnectionHandler.o bin/bookLibrary-Book.o bin/bookLibrary-BookCollection.o bin/bookLibrary-BookLibraryUser.o bin/bookLibrary-Main.o bin/bookLibrary-UserBooks.o bin/stomp-Frame.o bin/stomp-StompConnectionHandler.o bin/stomp-StompMessageEncoderDecoder.o bin/Printer.o bin/Util.o

bin/api-ConnectionHandler.o: src/api/ConnectionHandler.cpp include/api/ConnectionHandler.h
	$(CC) $(CFLAGS) -o bin/api-ConnectionHandler.o src/api/ConnectionHandler.cpp

bin/bookLibrary-Book.o: src/bookLibrary/Book.cpp include/bookLibrary/Book.h
	$(CC) $(CFLAGS) -o bin/bookLibrary-Book.o src/bookLibrary/Book.cpp

bin/bookLibrary-BookCollection.o: src/bookLibrary/BookCollection.cpp include/bookLibrary/BookCollection.h
	$(CC) $(CFLAGS) -o bin/bookLibrary-BookCollection.o src/bookLibrary/BookCollection.cpp

bin/bookLibrary-BookLibraryUser.o: src/bookLibrary/BookLibraryUser.cpp include/bookLibrary/BookLibraryUser.h
	$(CC) $(CFLAGS) -o bin/bookLibrary-BookLibraryUser.o src/bookLibrary/BookLibraryUser.cpp

bin/bookLibrary-Main.o: src/bookLibrary/Main.cpp include/bookLibrary/Main.h
	$(CC) $(CFLAGS) -o bin/bookLibrary-Main.o src/bookLibrary/Main.cpp

bin/bookLibrary-UserBooks.o: src/bookLibrary/UserBooks.cpp include/bookLibrary/UserBooks.h
	$(CC) $(CFLAGS) -o bin/bookLibrary-UserBooks.o src/bookLibrary/UserBooks.cpp

bin/stomp-Frame.o: src/stomp/Frame.cpp include/stomp/Frame.h
	$(CC) $(CFLAGS) -o bin/stomp-Frame.o src/stomp/Frame.cpp

bin/stomp-StompConnectionHandler.o: src/stomp/StompConnectionHandler.cpp include/stomp/StompConnectionHandler.h
	$(CC) $(CFLAGS) -o bin/stomp-StompConnectionHandler.o src/stomp/StompConnectionHandler.cpp

bin/stomp-StompMessageEncoderDecoder.o: src/stomp/StompMessageEncoderDecoder.cpp include/stomp/StompMessageEncoderDecoder.h
	$(CC) $(CFLAGS) -o bin/stomp-StompMessageEncoderDecoder.o src/stomp/StompMessageEncoderDecoder.cpp

bin/Printer.o: src/Printer.cpp include/Printer.h
	$(CC) $(CFLAGS) -o bin/Printer.o src/Printer.cpp

bin/Util.o: src/Util.cpp include/Util.h
	$(CC) $(CFLAGS) -o bin/Util.o src/Util.cpp

bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -o bin/main.o src/main.cpp

clean:
	rm -f bin/*.o StompBookClubClient
