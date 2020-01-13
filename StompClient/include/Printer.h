
#ifndef STOMPCLIENT_PRINTER_H
#define STOMPCLIENT_PRINTER_H

#include <string>
#include "Lock.h"

class Printer {
private:
    std::mutex _lock;

public:
    Printer();

    void print(const std::string &s);
    void println(const std::string &s);
};

#endif //STOMPCLIENT_PRINTER_H
