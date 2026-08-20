#include "fit_table_copy.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/squote.h"
#include "engine/quote_ident.h"
#include "engine/text_rows.h"
#include "engine/has_error.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database {

def with_table_copy() {
    return {
        "sql.table-copy",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_table_copy(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, table, to, into;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{}) : std::string{};
                to     = p.is_object() ? p.value("to", std::string{}) : std::string{};
                into   = p.is_object() ? p.value("into", table) : table;
            } catch (const std::exception&) {}
            if (into.empty()) into = table;

            sqlite3* db = db_open(target);
            if (!db) { send_to(options, json{{"ok", false}, {"error", "cannot open db: " + target}}, {}); return true; }
            exec(db, "ATTACH DATABASE '" + squote(to) + "' AS copydest");
            exec(db, "DROP TABLE IF EXISTS copydest." + quote_ident(into));
            const json frames = exec(db, "CREATE TABLE copydest." + quote_ident(into) + " AS SELECT * FROM " + quote_ident(table));
            long rows = 0;
            const auto cnt = text_rows(db, "SELECT COUNT(*) FROM copydest." + quote_ident(into));
            if (!has_error(frames) && !cnt.empty() && !cnt[0].empty()) rows = std::stol(cnt[0][0]);
            exec(db, "DETACH DATABASE copydest");
            db_close(db);

            if (has_error(frames)) { send_to(options, json{{"ok", false}, {"frames", frames}}, {}); return true; }
            send_to(options, json{{"ok", true}, {"table", into}, {"rows", rows}, {"to", to}}, {});
            return 1;
        },
    };
}

}
