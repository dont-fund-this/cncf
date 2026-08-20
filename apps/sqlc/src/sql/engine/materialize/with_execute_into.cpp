#include "engine/materialize/fit_execute_into.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "engine/materialize/materialize_each.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace sql::engine {

def with_execute_into() {
    return {
        "sql.execute-into",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_execute_into(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, sql, results;
            try {
                target  = p.is_object() ? p.value("target", std::string{}) : std::string{};
                sql     = p.is_object() ? p.value("sql", std::string{}) : std::string{};
                results = p.is_object() ? p.value("results", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            const bool attached = !results.empty() && results != target;
            std::string prefix;
            if (attached) { exec(db, "ATTACH DATABASE '" + squote(results) + "' AS rdb"); prefix = "rdb."; }
            sqlite3_exec(db, ("CREATE TABLE IF NOT EXISTS " + prefix + "ExecuteInfo(rowid INTEGER PRIMARY KEY, TimeStamp TEXT, Text TEXT)").c_str(), nullptr, nullptr, nullptr);
            sqlite3_exec(db, ("CREATE TABLE IF NOT EXISTS " + prefix + "ResultInfo(rowid INTEGER PRIMARY KEY, ExecuteId INTEGER, ResultName TEXT)").c_str(), nullptr, nullptr, nullptr);
            exec(db, "INSERT INTO " + prefix + "ExecuteInfo(TimeStamp, Text) VALUES (datetime('now'),'" + squote(sql) + "')");
            const long execute_id = sqlite3_last_insert_rowid(db);
            const std::vector<std::string> names = materialize_each(db, prefix, execute_id, sql);
            if (attached) exec(db, "DETACH DATABASE rdb");
            db_close(db);

            send_to(options, json{{"ok", true}, {"execute", execute_id}, {"results", names}}, {});
            return 1;
        },
    };
}
}
