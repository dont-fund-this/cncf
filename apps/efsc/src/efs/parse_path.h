#pragma once

#include "parse_payload.h"
#include "require_path.h"

#include <string>

inline std::string parse_path(const char* payload, std::string& err) {
    auto j = parse_payload(payload, err);
    if (!err.empty()) return {};
    return require_path(j, err);
}
