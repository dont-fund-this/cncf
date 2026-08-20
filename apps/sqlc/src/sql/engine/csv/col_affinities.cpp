#include "engine/csv/col_affinities.h"
#include "engine/csv/affinity_of.h"

std::vector<char> col_affinities(const std::string& schema) {
    std::vector<char> out;
    const auto lp = schema.find('(');
    const auto rp = schema.rfind(')');
    if (lp == std::string::npos || rp == std::string::npos || rp <= lp) return out;

    std::string cur;
    int depth = 0;
    for (size_t i = lp + 1; i < rp; ++i) {
        const char c = schema[i];
        if (c == '(') ++depth;
        if (c == ')') --depth;
        if (c == ',' && depth == 0) { out.push_back(affinity_of(cur)); cur.clear(); continue; }
        cur += c;
    }
    if (!cur.empty()) out.push_back(affinity_of(cur));
    return out;
}
