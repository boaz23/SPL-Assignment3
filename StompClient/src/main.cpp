#include "../include/bookLibrary/Main.h"

using std::string;

int main (int argc, char *argv[]) {
    Printer printer;
    Main main(printer);
    main.start();
    return 0;
}
