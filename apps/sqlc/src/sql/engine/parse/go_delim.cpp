#include "engine/parse/go_delim.h"
#include "engine/parse/trim.h"

#include <algorithm>
#include <cctype>

bool go_delim(const std::string& line) {
    std::string t = trim(line);
    std::transform(t.begin(), t.end(), t.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (t.empty() || t[0] != '-' || t.find("go") == std::string::npos) return false;
    std::string reduced;
    for (char c : t) if (c != '-' && c != ' ') reduced += c;
    return reduced == "go";
}
