#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> text_rows(sqlite3* db, const std::string& sql);
