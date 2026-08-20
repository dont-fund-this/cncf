#include "fit_schema.h"
#include "state.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "impl/schema_filter.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::meta {

def with_schema() {
    return {
        "sql.schema",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_schema(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, sort;
            long limit = -1, offset = 0;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                sort   = p.is_object() ? p.value("sort", std::string{})   : std::string{};
                limit  = p.is_object() ? p.value("limit", -1) : -1;
                offset = p.is_object() ? p.value("offset", 0) : 0;
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }

            std::string sql = "SELECT type, name, tbl_name, rootpage, sql FROM sqlite_master "
                              "WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%'";
            sql += schema_filter(sql::state().filters);
            sql += (sort == "name") ? " ORDER BY name" : " ORDER BY type, name";
            if (limit >= 0) sql += " LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);

            const json frames = exec(db, sql);
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
