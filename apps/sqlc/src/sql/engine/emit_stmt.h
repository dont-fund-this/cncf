#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

nlohmann::json emit_stmt(sqlite3_stmt* st);
