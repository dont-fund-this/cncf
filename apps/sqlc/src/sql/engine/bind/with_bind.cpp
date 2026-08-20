#include "engine/bind/fit_bind.h"
#include "engine/bind/bind_exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_bind() {
    return {
        "sql.bind",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_bind(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string sql, target;
            json params = json::array();
            try {
                sql    = p.is_object() ? p.value("sql", std::string{})    : std::string{};
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                if (p.is_object() && p.contains("params")) params = p["params"];
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }

            const json frames = bind_exec(db, sql, params);
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
