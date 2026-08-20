#include "engine/bind/bind_exec.h"
#include "engine/bind/bind_params.h"
#include "engine/emit_stmt.h"
#include "engine/now_iso.h"

nlohmann::json bind_exec(sqlite3* db, const std::string& sql, const nlohmann::json& params) {
    using nlohmann::json;
    json frames = json::array();

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &st, nullptr) != SQLITE_OK) {
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "prepare"},
                          {"text", sqlite3_errmsg(db)}, {"stmt", sql}, {"at", now_iso()}});
        return frames;
    }

    bind_params(st, params);
    frames = emit_stmt(st);
    sqlite3_finalize(st);
    return frames;
}
