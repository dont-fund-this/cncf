#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <string>

nlohmann::json bind_exec(sqlite3* db, const std::string& sql, const nlohmann::json& params);
