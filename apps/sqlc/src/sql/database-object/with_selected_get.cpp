#include "fit_selected_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

namespace sql::database_object {

def with_selected_get() {
    return {
        "sql.selected-object-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            auto& st = sql::state();
            send_to(options, json{
                {"name", st.selected_object.empty() ? json(nullptr) : json(st.selected_object)},
                {"type", st.selected_object_type.empty() ? json(nullptr) : json(st.selected_object_type)}}, {});
            return 1;
        },
    };
}

}
