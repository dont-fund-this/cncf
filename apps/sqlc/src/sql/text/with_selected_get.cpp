#include "fit_selected_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::text {

def with_selected_get() {
    return {
        "sql.selected-text-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            send_to(options, json{{"text", sql::state().selected_text}}, {});
            return 1;
        },
    };
}

}
