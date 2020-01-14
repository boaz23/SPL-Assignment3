#include <iostream>
#include "../include/Printer.h"


Printer::Printer() : _lock() { }

void Printer::print(const std::string &s) {
    mutex_lock lock(_lock);
    if (s[s.length() - 1] == '\0') {
        std::cout << s.substr(0, s.length() - 1);
    }
    else {
        std::cout << s;
    }
}

void Printer::println(const std::string &s) {
    if (s.length() >= 2 && s[s.length() - 1] == '\0' && s[s.length() - 2] == '\0') {
        const std::string tmp = s.substr(0, s.length() - 1);
        mutex_lock lock(_lock);
        std::cout << tmp << std::endl;
    }
    else {
        mutex_lock lock(_lock);
        std::cout << s << std::endl;
    }
}
