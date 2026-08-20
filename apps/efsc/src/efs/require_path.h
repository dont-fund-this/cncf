#pragma once

#include <nlohmann/json.hpp>
#include <string>

inline std::string require_path(const nlohmann::json& j, std::string& err) {
    auto path = j.value("path", std::string{});
    if (path.empty()) err = "missing path";
    return path;
}
