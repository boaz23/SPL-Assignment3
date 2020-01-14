#include "../include/bookLibrary/Main.h"

using std::string;

// TODO: write documentations and refactor

int main (int argc, char *argv[]) {
    StompMessageEncoderDecoder encdec;
    StompConnectionHandler connection(encdec);
    Printer printer;
    Main main(encdec, connection, printer);
    main.start();
    return 0;
}
