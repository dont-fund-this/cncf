#include "fit_list_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::result {

def with_list_get() {
    return {
        "sql.result-list-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_list_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            send_to(options, json{{"count", static_cast<int>(st.results.size())}, {"selected", st.selected_result >= 0 ? json(st.selected_result) : json(nullptr)}}, {});
            return 1;
        },
    };
}

}
