#include "engine/parse/statements.h"
#include "engine/parse/go_delim.h"
#include "engine/parse/split_on_go.h"
#include "engine/parse/split_on_semicolon.h"

std::vector<std::string> statements(const std::string& sql) {
    std::vector<std::string> lines;
    std::string cur;
    for (char c : sql) {
        if (c == '\n') { lines.push_back(cur); cur.clear(); }
        else cur += c;
    }
    lines.push_back(cur);

    for (const auto& line : lines) {
        if (go_delim(line)) return split_on_go(lines);
    }
    return split_on_semicolon(sql);
}
