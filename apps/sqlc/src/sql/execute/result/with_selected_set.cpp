#include "fit_selected_set.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::result {

def with_selected_set() {
    return {
        "sql.selected-result-set",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_set(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            int index = -1;
            try { index = p.is_object() ? p.value("index", -1) : -1; } catch (const std::exception&) {}

            auto& st = sql::state();
            const bool ok = index >= 0 && index < static_cast<int>(st.results.size());
            if (ok) st.selected_result = index;
            send_to(options, json{{"ok", ok}, {"selected", ok ? json(index) : json(nullptr)}}, {});
            return 1;
        },
    };
}

}
