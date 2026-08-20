#include "fit_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::filter {

def with_get() {
    return {
        "sql.filters-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            send_to(options, json{{"filters", sql::state().filters}}, {});
            return 1;
        },
    };
}

}
