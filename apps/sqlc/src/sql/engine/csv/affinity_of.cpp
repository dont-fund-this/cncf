#include "engine/csv/affinity_of.h"

#include <algorithm>
#include <cctype>

char affinity_of(const std::string& column_def) {
    const size_t b = column_def.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return 't';
    const size_t sp = column_def.find_first_of(" \t", b);
    std::string type = (sp == std::string::npos) ? std::string{} : column_def.substr(sp + 1);
    std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) { return std::toupper(c); });
    if (type.find("INT") != std::string::npos) return 'i';
    if (type.find("REAL") != std::string::npos || type.find("FLOA") != std::string::npos || type.find("DOUB") != std::string::npos) return 'r';
    return 't';
}
