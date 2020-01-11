#include <iostream>
#include "../include/Printer.h"


Printer::Printer() : _lock() { }

void Printer::print(const std::string &s) {
    mutex_lock lock(_lock);
    std::cout << s;
}

void Printer::println(const std::string &s) {
    mutex_lock lock(_lock);
    std::cout << s << std::endl;
}
