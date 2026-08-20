#include "engine/parse/split_on_semicolon.h"
#include "engine/parse/push_trimmed.h"

std::vector<std::string> split_on_semicolon(const std::string& sql) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : sql) {
        if (c == ';') { push_trimmed(out, cur); continue; }
        cur += c;
    }
    push_trimmed(out, cur);
    return out;
}
