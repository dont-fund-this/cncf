#include "engine/materialize/fit_materialize.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/quote_ident.h"
#include "engine/text_rows.h"
#include "engine/has_error.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_materialize() {
    return {
        "sql.materialize",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_materialize(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, sql, name;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                sql    = p.is_object() ? p.value("sql", std::string{}) : std::string{};
                name   = p.is_object() ? p.value("name", std::string("result")) : std::string("result");
            } catch (const std::exception&) {}
            if (name.empty()) name = "result";

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            exec(db, "DROP TABLE IF EXISTS " + quote_ident(name));
            const json frames = exec(db, "CREATE TABLE " + quote_ident(name) + " AS " + sql);
            long rows = 0;
            const auto cnt = text_rows(db, "SELECT COUNT(*) FROM " + quote_ident(name));
            if (!has_error(frames) && !cnt.empty() && !cnt[0].empty()) rows = std::stol(cnt[0][0]);
            db_close(db);

            if (has_error(frames)) { send_to(options, json{{"ok", false}, {"frames", frames}}, {}); return true; }
            send_to(options, json{{"ok", true}, {"table", name}, {"rows", rows}}, {});
            return 1;
        },
    };
}
}
