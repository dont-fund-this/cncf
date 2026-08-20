#include "fit_save.h"
#include "state.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::settings {

def with_save() {
    return {
        "sql.settings-save",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_save(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target;
            try { target = p.is_object() ? p.value("target", std::string{}) : std::string{}; } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            auto& st = sql::state();
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Connections(name TEXT, target TEXT, cohort TEXT)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Options(name TEXT, value TEXT)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "DELETE FROM Connections", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "DELETE FROM Options", nullptr, nullptr, nullptr);
            for (const auto& e : st.quiver) {
                const std::string n = e.value("name", std::string{});
                const std::string t = e.value("target", std::string{});
                const std::string c = e.value("cohort", std::string{});
                exec(db, "INSERT INTO Connections VALUES ('" + squote(n) + "','" + squote(t) + "','" + squote(c) + "')");
            }
            for (const auto& kv : st.options)
                exec(db, "INSERT INTO Options VALUES ('" + squote(kv.first) + "','" + squote(kv.second) + "')");
            db_close(db);
            send_to(options, json{{"ok", true}, {"connections", st.quiver.size()}, {"options", st.options.size()}}, {});
            return 1;
        },
    };
}

}
