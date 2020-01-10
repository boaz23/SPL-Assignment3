

#ifndef STOMPCLIENT_UTIL_H
#define STOMPCLIENT_UTIL_H

#include <vector>
#include <string>

class Util {
public:
    static std::vector<std::string> split(const std::string &s, const std::string &delimter);
};


#endif
