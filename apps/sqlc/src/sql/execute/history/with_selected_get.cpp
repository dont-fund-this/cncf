#include "fit_selected_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::history {

def with_selected_get() {
    return {
        "sql.selected-execute-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            const bool has = st.selected_execute >= 0 && st.selected_execute < static_cast<int>(st.history.size());
            send_to(options, json{
                {"index", has ? json(st.selected_execute) : json(nullptr)},
                {"sql", has ? json(st.history[st.selected_execute]) : json(nullptr)}}, {});
            return 1;
        },
    };
}

}
