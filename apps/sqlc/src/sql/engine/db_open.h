#pragma once

#include <sqlite3.h>
#include <string>

sqlite3* db_open(const std::string& target);
