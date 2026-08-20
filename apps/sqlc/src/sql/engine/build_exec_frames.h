#pragma once

#include <nlohmann/json.hpp>
#include <string>

nlohmann::json build_exec_frames(const std::string& sql, const std::string& target, bool counters);
