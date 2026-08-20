#include "engine/csv/csv_parse.h"
#include "engine/csv/fields.h"

void csv_parse(std::string_view line, char delim, std::vector<std::string_view>& out, std::deque<std::string>& owned) {
    if (line.find('"') == std::string_view::npos) { fields(line, out, delim); return; }

    out.clear();
    std::string cur;
    bool inq = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (inq && c == '"' && i + 1 < line.size() && line[i + 1] == '"') { cur += '"'; ++i; continue; }
        if (inq && c == '"') { inq = false; continue; }
        if (inq)             { cur += c; continue; }
        if (c == '"')        { inq = true; continue; }
        if (c == delim)      { owned.push_back(cur); out.push_back(owned.back()); cur.clear(); continue; }
        cur += c;
    }
    owned.push_back(cur);
    out.push_back(owned.back());
}
