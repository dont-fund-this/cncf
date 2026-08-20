#include "fit_add.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::filter {

def with_add() {
    return {
        "sql.filter-add",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_add(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            const json filter = (p.is_object() && p.contains("filter")) ? p["filter"] : json::object();
            sql::state().filters.push_back(filter);
            send_to(options, json{{"ok", true}, {"count", static_cast<int>(sql::state().filters.size())}}, {});
            return 1;
        },
    };
}

}
