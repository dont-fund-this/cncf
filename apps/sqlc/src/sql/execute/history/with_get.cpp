#include "fit_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::history {

def with_get() {
    return {
        "sql.execute-history-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            send_to(options, json{{"history", st.history}, {"selected", st.selected_execute >= 0 ? json(st.selected_execute) : json(nullptr)}}, {});
            return 1;
        },
    };
}

}
