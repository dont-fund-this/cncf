#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

void bind_params(sqlite3_stmt* st, const nlohmann::json& params);
