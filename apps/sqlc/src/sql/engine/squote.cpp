#include "engine/squote.h"

std::string squote(const std::string& s) {
    std::string out;
    for (char c : s) { if (c == '\'') out += '\''; out += c; }
    return out;
}
