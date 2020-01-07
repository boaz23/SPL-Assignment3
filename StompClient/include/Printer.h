
#ifndef STOMPCLIENT_PRINTER_H
#define STOMPCLIENT_PRINTER_H

#include <string>

class Printer {
    // TODO: implement printing to std::cout and std::err with mutex

    void print(std::string s);
    void println(std::string s);
};

#endif //STOMPCLIENT_PRINTER_H
