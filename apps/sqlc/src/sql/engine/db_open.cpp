#include "engine/db_open.h"
#include "vtab/csv.h"

sqlite3* db_open(const std::string& target) {
    const std::string path = (target.empty() || target == "memory") ? ":memory:" : target;
    sqlite3* db = nullptr;
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        sqlite3_close(db);
        return nullptr;
    }
    register_csv(db);
    return db;
}
