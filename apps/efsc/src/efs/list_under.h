#pragma once

#include <nlohmann/json.hpp>
#include <string>

nlohmann::json list_under(const std::string& path);
