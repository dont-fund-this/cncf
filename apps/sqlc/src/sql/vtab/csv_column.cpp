#include "csv.h"

#include <cstdlib>
#include <string>

int csv_column(sqlite3_vtab_cursor* cur, sqlite3_context* ctx, int i) {
    CsvCursor* c = reinterpret_cast<CsvCursor*>(cur);
    if (i < 0 || i >= static_cast<int>(c->row.size())) { sqlite3_result_null(ctx); return SQLITE_OK; }
    const std::string_view f = c->row[i];
    const char a = (i < static_cast<int>(c->vt->aff.size())) ? c->vt->aff[i] : 't';
    if (a != 't' && f.empty()) { sqlite3_result_null(ctx); return SQLITE_OK; }
    if (a == 'i') { sqlite3_result_int64(ctx, std::strtoll(std::string(f).c_str(), nullptr, 10)); return SQLITE_OK; }
    if (a == 'r') { sqlite3_result_double(ctx, std::strtod(std::string(f).c_str(), nullptr)); return SQLITE_OK; }
    sqlite3_result_text(ctx, f.data(), static_cast<int>(f.size()), SQLITE_STATIC);
    return SQLITE_OK;
}
