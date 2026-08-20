#include "fit_clear.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::filter {

def with_clear() {
    return {
        "sql.filters-clear",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_clear(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            sql::state().filters.clear();
            send_to(options, json{{"ok", true}}, {});
            return 1;
        },
    };
}

}
