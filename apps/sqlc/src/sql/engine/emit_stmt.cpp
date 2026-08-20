#include "engine/emit_stmt.h"
#include "engine/now_iso.h"
#include "engine/read/columns.h"
#include "engine/read/coltypes.h"
#include "engine/read/rows.h"

nlohmann::json emit_stmt(sqlite3_stmt* st) {
    using nlohmann::json;
    json frames = json::array();
    if (!st) return frames;

    const int ncol = sqlite3_column_count(st);
    if (ncol > 0) {
        frames.push_back({{"kind", "resultset"}, {"columns", columns(st, ncol)}, {"types", coltypes(st, ncol)}});
        frames.push_back({{"kind", "rows"}, {"rows", rows(st, ncol)}});
        return frames;
    }

    int rc;
    while ((rc = sqlite3_step(st)) == SQLITE_ROW) {}
    if (rc != SQLITE_DONE)
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "step"},
                          {"text", sqlite3_errmsg(sqlite3_db_handle(st))}, {"stmt", sqlite3_sql(st)}, {"at", now_iso()}});
    return frames;
}
