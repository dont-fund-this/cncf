#pragma once

#include <sqlite3.h>
#include <nlohmann/json.hpp>
#include <string>

nlohmann::json exec(sqlite3* db, const std::string& sql);
