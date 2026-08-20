#pragma once

#include <sqlite3.h>
#include <nlohmann/json.hpp>

nlohmann::json row(sqlite3_stmt* st, int ncol);
