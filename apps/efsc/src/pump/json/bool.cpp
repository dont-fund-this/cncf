#include "../../type.hpp"
#include <string_view>

bool json_parse_bool(std::string_view s, bool* out) {
    if (s == "true") { if (out) *out = true; return true; }
    if (s == "false") { if (out) *out = false; return false; }
    return false;
}
