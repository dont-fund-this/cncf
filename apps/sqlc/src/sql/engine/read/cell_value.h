#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

nlohmann::json cell_value(sqlite3_stmt* st, int i, int type);
