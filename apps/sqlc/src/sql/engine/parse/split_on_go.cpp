#include "engine/parse/split_on_go.h"
#include "engine/parse/go_delim.h"
#include "engine/parse/push_trimmed.h"

std::vector<std::string> split_on_go(const std::vector<std::string>& lines) {
    std::vector<std::string> out;
    std::string cur;
    for (const auto& line : lines) {
        if (go_delim(line)) { push_trimmed(out, cur); continue; }
        if (!cur.empty()) cur += '\n';
        cur += line;
    }
    push_trimmed(out, cur);
    return out;
}
