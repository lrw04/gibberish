#include "word.h"

std::string regular(const std::string& s) {
    std::string res;
    for (const auto& c : s)
        if (c >= 'A' && c <= 'Z')
            res += (c - 'A' + 'a');
        else
            res += c;
    return res;
}

bool is_punct(char c) {
    return c == '!' || c == ';' || c == ':' || c == ',' || c == '.' ||
           c == '?' || c == '`' || c == '\'' || c == '"';
}
