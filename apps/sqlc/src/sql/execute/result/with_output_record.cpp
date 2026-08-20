#include "fit_output_record.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::execute::result {

def with_output_record() {
    return {
        "sql.output-record",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_output_record(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, text;
            json frames = json::array();
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                text   = p.is_object() ? p.value("text", std::string{})   : std::string{};
                frames = p.is_object() ? p.value("frames", json::array())  : json::array();
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ExecuteInfo(rowid INTEGER PRIMARY KEY, TimeStamp TEXT, Text TEXT)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ExecuteDetails(rowid INTEGER PRIMARY KEY, ExecuteId INTEGER, DetailText TEXT, DetailInt INTEGER)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ErrorInfo(rowid INTEGER PRIMARY KEY, ExecuteId INTEGER, Code TEXT, Message TEXT, Stmt TEXT, At TEXT)", nullptr, nullptr, nullptr);
            exec(db, "INSERT INTO ExecuteInfo(TimeStamp, Text) VALUES (datetime('now'),'" + squote(text) + "')");
            const long id = sqlite3_last_insert_rowid(db);
            for (const auto& f : frames) {
                const std::string kind = f.value("kind", std::string{});
                if (kind == "info")
                    exec(db, "INSERT INTO ExecuteDetails(ExecuteId, DetailText, DetailInt) VALUES (" + std::to_string(id) + ",'rows affected'," + std::to_string(f.value("changes", 0)) + ")");
                if (kind == "error")
                    exec(db, "INSERT INTO ErrorInfo(ExecuteId, Code, Message, Stmt, At) VALUES (" + std::to_string(id) + ",'" + squote(f.value("code", std::string{})) + "','" + squote(f.value("text", std::string{})) + "','" + squote(f.value("stmt", std::string{})) + "','" + squote(f.value("at", std::string{})) + "')");
            }
            db_close(db);
            send_to(options, json{{"ok", true}, {"id", id}}, {});
            return 1;
        },
    };
}

}
