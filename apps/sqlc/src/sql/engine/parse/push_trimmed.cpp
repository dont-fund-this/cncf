#include "engine/parse/push_trimmed.h"
#include "engine/parse/trim.h"

void push_trimmed(std::vector<std::string>& out, std::string& cur) {
    const std::string stmt = trim(cur);
    if (!stmt.empty()) out.push_back(stmt);
    cur.clear();
}
