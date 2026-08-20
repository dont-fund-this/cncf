#include "engine/text_rows.h"

std::vector<std::vector<std::string>> text_rows(sqlite3* db, const std::string& sql) {
    std::vector<std::vector<std::string>> out;
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &st, nullptr) != SQLITE_OK) return out;

    const int ncol = sqlite3_column_count(st);
    while (sqlite3_step(st) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < ncol; i++) {
            const unsigned char* t = sqlite3_column_text(st, i);
            row.push_back(t ? reinterpret_cast<const char*>(t) : "");
        }
        out.push_back(row);
    }
    sqlite3_finalize(st);
    return out;
}
