#include "fit_log_add.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::settings {

def with_log_add() {
    return {
        "sql.log-add",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_log_add(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, text;
            long code = 0;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                text   = p.is_object() ? p.value("text", std::string{})   : std::string{};
                code   = p.is_object() ? p.value("code", 0) : 0;
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {});
                return true;
            }
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS LogData(LogTime TEXT, LogCode INTEGER, LogText TEXT)", nullptr, nullptr, nullptr);
            const json frames = exec(db, "INSERT INTO LogData VALUES (datetime('now'), " + std::to_string(code) + ", '" + squote(text) + "')");
            db_close(db);

            bool ok = true;
            for (const auto& f : frames) if (f.value("kind", std::string{}) == "error") ok = false;
            send_to(options, json{{"ok", ok}}, {});
            return 1;
        },
    };
}

}
