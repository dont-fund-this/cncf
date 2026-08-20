#include "engine/read/row.h"
#include "engine/read/type_name.h"
#include "engine/read/cell_value.h"

#include <sqlite3.h>

nlohmann::json row(sqlite3_stmt* st, int ncol) {
    nlohmann::json out = nlohmann::json::array();
    for (int i = 0; i < ncol; ++i) {
        const int t = sqlite3_column_type(st, i);
        nlohmann::json cell;
        cell["t"] = type_name(t);
        cell["v"] = cell_value(st, i, t);
        out.push_back(cell);
    }
    return out;
}
