#include "engine/read/rows.h"
#include "engine/read/row.h"

nlohmann::json rows(sqlite3_stmt* st, int ncol) {
    nlohmann::json out = nlohmann::json::array();
    while (sqlite3_step(st) == SQLITE_ROW) out.push_back(row(st, ncol));
    return out;
}
