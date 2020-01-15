#include "../include/Util.h"

std::vector<std::string> Util::split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::string Util::rebuildString(const std::vector<std::string> &strings, int startIndex) {
    return rebuildString(strings.begin() + startIndex, strings.end());
}

std::string Util::rebuildString(const std::vector<std::string>::const_iterator &start,
                                const std::vector<std::string>::const_iterator &end) {
    std::string s;
    if (start < end) {
        auto word = start;
        s.append(*word);
        ++word;
        for (; word != end; ++word) {
            s.append(" ")
                .append(*word);
        }
    }

    return s;
}

template<typename T>
std::string Util::nextId(T &id) {
    return std::to_string(id++);
}

template std::string Util::nextId<int>(int&);