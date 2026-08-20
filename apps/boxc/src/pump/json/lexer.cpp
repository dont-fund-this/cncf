#include "../../type.hpp"
#include <string_view>
#include <optional>

size_t json_skip_whitespace(std::string_view s, size_t i) {
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) {
        i++;
    }
    return i;
}

size_t json_skip_string(std::string_view s, size_t i) {
    if (i >= s.size() || s[i] != '"') return i;
    i++;
    while (i < s.size()) {
        if (s[i] == '\\') {
            i += 2;
        } else if (s[i] == '"') {
            return i + 1;
        } else {
            i++;
        }
    }
    return i;
}

size_t json_skip_value(std::string_view s, size_t i) {
    i = json_skip_whitespace(s, i);
    if (i >= s.size()) return i;
    if (s[i] == '"') return json_skip_string(s, i);
    if (s[i] == '{') {
        int depth = 1;
        i++;
        while (i < s.size() && depth > 0) {
            if (s[i] == '"') {
                i = json_skip_string(s, i);
            } else {
                if (s[i] == '{') depth++;
                else if (s[i] == '}') depth--;
                i++;
            }
        }
        return i;
    }
    if (s[i] == '[') {
        int depth = 1;
        i++;
        while (i < s.size() && depth > 0) {
            if (s[i] == '"') {
                i = json_skip_string(s, i);
            } else {
                if (s[i] == '[') depth++;
                else if (s[i] == ']') depth--;
                i++;
            }
        }
        return i;
    }
    while (i < s.size() && s[i] != ',' && s[i] != '}' && s[i] != ']' && s[i] != ' ' && s[i] != '\n' && s[i] != '\r' && s[i] != '\t') {
        i++;
    }
    return i;
}

std::optional<std::string_view> json_find_field(std::string_view s, std::string_view key) {
    size_t i = json_skip_whitespace(s, 0);
    if (i >= s.size() || s[i] != '{') return std::nullopt;
    i++;

    while (i < s.size()) {
        i = json_skip_whitespace(s, i);
        if (i >= s.size() || s[i] == '}') break;
        if (s[i] != '"') break;

        size_t kstart = i + 1;
        i = json_skip_string(s, i);
        size_t kend = (i > 0 && s[i - 1] == '"') ? i - 1 : i;

        i = json_skip_whitespace(s, i);
        if (i >= s.size() || s[i] != ':') break;
        i++;
        i = json_skip_whitespace(s, i);

        size_t vstart = i;
        size_t vend = json_skip_value(s, i);

        if (s.substr(kstart, kend - kstart) == key) {
            if (vstart < s.size() && s[vstart] == '"' && vend > vstart && s[vend - 1] == '"') {
                return s.substr(vstart + 1, (vend - 1) - (vstart + 1));
            }
            return s.substr(vstart, vend - vstart);
        }

        i = vend;
        i = json_skip_whitespace(s, i);
        if (i < s.size() && s[i] == ',') i++;
    }
    return std::nullopt;
}
