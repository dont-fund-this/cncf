#include "fit_columns.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::meta {

def with_columns() {
    return {
        "sql.columns",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_columns(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, table;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{})  : std::string{};
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }

            const json frames = exec(db, "PRAGMA table_info('" + squote(table) + "')");
            db_close(db);

            int rc = 0;
            for (const auto& f : frames) {
                if (f.value("kind", std::string{}) == "error") rc = 1;
                send_to(options, f, {});
            }
            send_to(options, json{{"kind", "done"}, {"rc", rc}}, {});
            return 1;
        },
    };
}

}
