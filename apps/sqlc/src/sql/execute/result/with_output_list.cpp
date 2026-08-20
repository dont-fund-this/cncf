#include "fit_output_list.h"
#include "engine/exec.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::execute::result {

def with_output_list() {
    return {
        "sql.output-list",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_output_list(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target;
            long limit = 100;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                limit  = p.is_object() ? p.value("limit", 100) : 100;
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ExecuteInfo(rowid INTEGER PRIMARY KEY, TimeStamp TEXT, Text TEXT)", nullptr, nullptr, nullptr);
            const json frames = exec(db, "SELECT rowid, TimeStamp, Text FROM ExecuteInfo ORDER BY rowid DESC LIMIT " + std::to_string(limit));
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
