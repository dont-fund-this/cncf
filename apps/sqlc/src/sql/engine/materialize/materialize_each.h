#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

std::vector<std::string> materialize_each(sqlite3* db, const std::string& prefix, long execute_id, const std::string& sql);
