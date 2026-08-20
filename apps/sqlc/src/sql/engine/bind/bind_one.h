#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

void bind_one(sqlite3_stmt* st, int idx, const nlohmann::json& v);
