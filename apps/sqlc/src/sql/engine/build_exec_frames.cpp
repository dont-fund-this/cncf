#include "engine/build_exec_frames.h"
#include "engine/parse/statements.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/exec.h"
#include "engine/has_error.h"

nlohmann::json build_exec_frames(const std::string& sql, const std::string& target, bool counters) {
    using nlohmann::json;
    const auto stmts = statements(sql);

    json frames = json::array();
    if (stmts.empty()) {
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "empty"},
                          {"text", "no statements"}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }

    sqlite3* db = db_open(target);
    if (!db) {
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "open"},
                          {"text", "cannot open db: " + target}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }

    for (const auto& stmt : stmts) {
        const json sf = exec(db, stmt);
        for (const auto& f : sf) frames.push_back(f);
        if (counters && !has_error(sf))
            frames.push_back({{"kind", "info"}, {"changes", sqlite3_changes(db)}, {"rowid", sqlite3_last_insert_rowid(db)}});
    }
    db_close(db);

    frames.push_back({{"kind", "done"}, {"rc", has_error(frames) ? 1 : 0}});
    return frames;
}
