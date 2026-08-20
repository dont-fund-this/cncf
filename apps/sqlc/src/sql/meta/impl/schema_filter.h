#pragma once

#include <nlohmann/json.hpp>
#include <string>

std::string schema_filter(const nlohmann::json& filters);
