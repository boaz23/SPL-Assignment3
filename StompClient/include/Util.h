

#ifndef STOMPCLIENT_UTIL_H
#define STOMPCLIENT_UTIL_H

#include <vector>
#include <string>

/**
 * Utility class
 * Contains general functions
 */
class Util {
public:

    /**
     * Split the string by the @param delimter
     * @param s
     * @param delimter
     * @return vector of strings, that was separated by @param delimter
     */
    static std::vector<std::string> split(const std::string &s, const std::string &delimter);

    /**
     * Concatenate each item from @param startIndex of the vector @param strings separated by space
     * to new string and return it.
     * @param strings
     * @param startIndex
     * @return string concatenation of the vector start from @param startIndex
     */
    static std::string rebuildString(const std::vector<std::string> &strings, int startIndex);

    /**
     * Concatenate each item from @param start to @param end separated by space
     * to new string and return it.
     * @param start
     * @param end
     * @return string concatenation of the vector start from @param start to @param end
     */
    static std::string rebuildString(const std::vector<std::string>::const_iterator &start, const std::vector<std::string>::const_iterator &end);

    /**
     * Increment the @param id
     * @tparam T
     * @param id
     * @return T
     */
    template <typename T> static std::string nextId(T &id);
};


#endif
