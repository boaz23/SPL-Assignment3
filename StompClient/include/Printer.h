
#ifndef STOMPCLIENT_PRINTER_H
#define STOMPCLIENT_PRINTER_H

#include <string>
#include "Lock.h"

/**
 * Printer class
 * synchronized print to std::cout
 */
class Printer {
private:
    std::mutex _lock;

public:
    Printer();

    void print(const std::string &s);

    /**
     * Print string
     * @param s - string to print
     */
    void println(const std::string &s);
};

#endif //STOMPCLIENT_PRINTER_H
