#include "fit_page.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "engine/quote_ident.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::execute::result {

def with_page() {
    return {
        "sql.result-page",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_page(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, table;
            long limit = 100, offset = 0;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{}) : std::string{};
                limit  = p.is_object() ? p.value("limit", 100) : 100;
                offset = p.is_object() ? p.value("offset", 0) : 0;
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }
            const json frames = exec(db, "SELECT * FROM " + quote_ident(table) + " LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset));
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
