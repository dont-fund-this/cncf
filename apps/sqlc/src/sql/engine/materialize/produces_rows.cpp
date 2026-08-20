#include "engine/materialize/produces_rows.h"

bool produces_rows(sqlite3* db, const std::string& sql) {
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &st, nullptr) != SQLITE_OK) return false;
    const bool rows = sqlite3_column_count(st) > 0;
    sqlite3_finalize(st);
    return rows;
}
