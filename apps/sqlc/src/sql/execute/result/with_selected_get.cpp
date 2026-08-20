#include "fit_selected_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::execute::result {

def with_selected_get() {
    return {
        "sql.selected-result-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            const bool has = st.selected_result >= 0 && st.selected_result < static_cast<int>(st.results.size());
            if (has) send_to(options, st.results[st.selected_result], {});
            else     send_to(options, json{{"columns", nullptr}, {"rows", nullptr}}, {});
            return 1;
        },
    };
}

}
