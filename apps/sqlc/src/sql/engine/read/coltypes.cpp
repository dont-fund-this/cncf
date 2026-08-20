#include "engine/read/coltypes.h"

nlohmann::json coltypes(sqlite3_stmt* st, int ncol) {
    nlohmann::json out = nlohmann::json::array();
    for (int i = 0; i < ncol; ++i) {
        const char* d = sqlite3_column_decltype(st, i);
        out.push_back(d ? nlohmann::json(d) : nlohmann::json(nullptr));
    }
    return out;
}
