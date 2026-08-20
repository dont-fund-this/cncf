#pragma once

#include <sqlite3.h>
#include <string>

bool produces_rows(sqlite3* db, const std::string& sql);
