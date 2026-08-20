#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

nlohmann::json coltypes(sqlite3_stmt* st, int ncol);
