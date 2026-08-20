#include "fit_copy.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "engine/has_error.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database {

def with_copy() {
    return {
        "sql.database-copy",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_copy(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string from, to;
            try {
                from = p.is_object() ? p.value("from", std::string{}) : std::string{};
                to   = p.is_object() ? p.value("to", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            sqlite3* db = db_open(from);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + from}}, {}); return true; }
            const json frames = exec(db, "VACUUM INTO '" + squote(to) + "'");
            db_close(db);

            if (has_error(frames)) { send_to(options, json{{"ok", false}, {"frames", frames}}, {}); return true; }
            send_to(options, json{{"ok", true}, {"to", to}}, {});
            return 1;
        },
    };
}

}
