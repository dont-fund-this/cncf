#pragma once

#include <sqlite3.h>
#include <nlohmann/json.hpp>

nlohmann::json rows(sqlite3_stmt* st, int ncol);
