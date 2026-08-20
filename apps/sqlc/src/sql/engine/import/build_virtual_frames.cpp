#include "engine/import/build_virtual_frames.h"
#include "engine/squote.h"
#include "engine/csv/tune.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/quote_ident.h"

#include <sqlite3.h>
#include <string>

nlohmann::json build_virtual_frames(const std::string& target,
                                    const std::string& table,
                                    const std::string& file,
                                    const Flags& flags) {
    using nlohmann::json;
    json frames = json::array();

    sqlite3* db = db_open(target);
    if (!db) {
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }
    tune(db, flags);

    const char delim = flags.delimiter.empty() ? ',' : flags.delimiter[0];
    std::string create = "CREATE VIRTUAL TABLE temp.csvsrc USING csv(filename='" + squote(file) + "', header=" + (flags.header ? "true" : "false");
    if (delim != ',') { create += ", delimiter='"; create += delim; create += "'"; }
    create += ")";
    if (sqlite3_exec(db, create.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        db_close(db);
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "vtab"}, {"text", "cannot map csv: " + file}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }

    if (flags.replace) sqlite3_exec(db, ("DELETE FROM " + quote_ident(table)).c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);
    const int rc = sqlite3_exec(db, ("INSERT INTO " + quote_ident(table) + " SELECT * FROM csvsrc").c_str(), nullptr, nullptr, nullptr);
    const long n = static_cast<long>(sqlite3_changes(db));
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    db_close(db);

    if (rc != SQLITE_OK) {
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "insert"}, {"text", "no such table: " + table}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }
    frames.push_back({{"kind", "info"}, {"text", "imported " + std::to_string(n)}});
    frames.push_back({{"kind", "done"}, {"rc", 0}});
    return frames;
}
