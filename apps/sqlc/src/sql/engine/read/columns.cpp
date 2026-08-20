#include "engine/read/columns.h"

nlohmann::json columns(sqlite3_stmt* st, int ncol) {
    nlohmann::json out = nlohmann::json::array();
    for (int i = 0; i < ncol; ++i) out.push_back(sqlite3_column_name(st, i));
    return out;
}
