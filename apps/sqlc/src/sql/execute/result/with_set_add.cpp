#include "fit_set_add.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::result {

def with_set_add() {
    return {
        "sql.result-set-add",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_set_add(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            json rs = json::object();
            if (p.is_object()) {
                rs["columns"] = p.value("columns", json::array());
                rs["rows"]    = p.value("rows", json::array());
            }
            sql::state().results.push_back(rs);
            send_to(options, json{{"ok", true}, {"id", static_cast<int>(sql::state().results.size()) - 1}}, {});
            return 1;
        },
    };
}

}
