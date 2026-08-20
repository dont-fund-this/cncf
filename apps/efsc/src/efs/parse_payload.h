#pragma once

#include <nlohmann/json.hpp>
#include <string>

inline nlohmann::json parse_payload(const char* payload, std::string& err) {
    try {
        return nlohmann::json::parse(payload && *payload ? payload : "{}");
    } catch (const std::exception&) {
        err = "invalid payload";
        return nullptr;
    }
}
