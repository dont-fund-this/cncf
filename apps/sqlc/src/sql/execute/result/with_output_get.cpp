#include "fit_output_get.h"
#include "engine/text_rows.h"
#include "engine/db_open.h"
#include "engine/db_close.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::execute::result {

def with_output_get() {
    return {
        "sql.output-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_output_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target;
            long id = 0;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                id     = p.is_object() ? p.value("id", 0) : 0;
            } catch (const std::exception&) {}

            sqlite3* db = db_open(target);
            if (!db) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ExecuteDetails(rowid INTEGER PRIMARY KEY, ExecuteId INTEGER, DetailText TEXT, DetailInt INTEGER)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ErrorInfo(rowid INTEGER PRIMARY KEY, ExecuteId INTEGER, Code TEXT, Message TEXT, Stmt TEXT, At TEXT)", nullptr, nullptr, nullptr);
            const std::string where = " WHERE ExecuteId=" + std::to_string(id);
            for (const auto& r : text_rows(db, "SELECT DetailText, DetailInt FROM ExecuteDetails" + where))
                send_to(options, json{{"kind", "detail"}, {"text", r[0]}, {"value", r[1]}}, {});
            for (const auto& r : text_rows(db, "SELECT Code, Message, Stmt, At FROM ErrorInfo" + where))
                send_to(options, json{{"kind", "error"}, {"code", r[0]}, {"text", r[1]}, {"stmt", r[2]}, {"at", r[3]}}, {});
            db_close(db);
            send_to(options, json{{"kind", "done"}, {"rc", 0}}, {});
            return 1;
        },
    };
}

}
