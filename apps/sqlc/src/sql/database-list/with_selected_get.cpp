#include "fit_selected_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::database_list {

def with_selected_get() {
    return {
        "sql.selected-database-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            send_to(options, json{{"selected", st.selected.empty() ? json(nullptr) : json(st.selected)}}, {});
            return 1;
        },
    };
}

}
