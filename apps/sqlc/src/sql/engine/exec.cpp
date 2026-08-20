#include "engine/exec.h"
#include "engine/now_iso.h"
#include "engine/emit_stmt.h"

nlohmann::json exec(sqlite3* db, const std::string& sql) {
    using nlohmann::json;
    json frames = json::array();

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &st, nullptr) != SQLITE_OK) {
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "prepare"},
                          {"text", sqlite3_errmsg(db)}, {"stmt", sql}, {"at", now_iso()}});
        return frames;
    }

    frames = emit_stmt(st);
    sqlite3_finalize(st);
    return frames;
}
