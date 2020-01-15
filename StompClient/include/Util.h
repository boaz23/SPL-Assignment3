

#ifndef STOMPCLIENT_UTIL_H
#define STOMPCLIENT_UTIL_H

#include <vector>
#include <string>

class Util {
public:
    static std::vector<std::string> split(const std::string &s, const std::string &delimter);
    static std::string rebuildString(const std::vector<std::string> &strings, int startIndex);
    static std::string rebuildString(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end);
    template <typename T> static std::string nextId(T &id);
};


#endif
