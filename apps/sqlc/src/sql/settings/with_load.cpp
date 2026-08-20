#include "fit_load.h"
#include "state.h"
#include "engine/text_rows.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::settings {

def with_load() {
    return {
        "sql.settings-load",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_load(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target;
            try { target = p.is_object() ? p.value("target", std::string{}) : std::string{}; } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Connections(name TEXT, target TEXT, cohort TEXT)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Options(name TEXT, value TEXT)", nullptr, nullptr, nullptr);
            auto& st = sql::state();
            st.quiver = json::array();
            for (const auto& r : text_rows(db, "SELECT name, target, cohort FROM Connections"))
                st.quiver.push_back(json{{"name", r[0]}, {"target", r[1]}, {"cohort", r[2]}});
            st.options.clear();
            for (const auto& r : text_rows(db, "SELECT name, value FROM Options"))
                st.options[r[0]] = r[1];
            db_close(db);
            send_to(options, json{{"ok", true}, {"connections", st.quiver.size()}, {"options", st.options.size()}}, {});
            return 1;
        },
    };
}

}
