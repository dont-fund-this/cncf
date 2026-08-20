#pragma once

#include <sqlite3.h>
#include <string_view>
#include <vector>

bool feed(sqlite3_stmt* st, const std::vector<std::string_view>& vals);
