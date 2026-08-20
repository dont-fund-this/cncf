#include "engine/quote_ident.h"

std::string quote_ident(std::string_view name) {
    std::string out = "\"";
    for (char c : name) {
        if (c == '"') out += '"';
        out += c;
    }
    out += '"';
    return out;
}
