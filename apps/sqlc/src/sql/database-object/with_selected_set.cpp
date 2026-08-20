#include "fit_selected_set.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_object {

def with_selected_set() {
    return {
        "sql.selected-object-set",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_set(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string name, type;
            try {
                name = p.is_object() ? p.value("name", std::string{}) : std::string{};
                type = p.is_object() ? p.value("type", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            auto& st = sql::state();
            st.selected_object = name;
            st.selected_object_type = type;
            send_to(options, json{{"ok", true}, {"name", name}, {"type", type}}, {});
            return 1;
        },
    };
}

}
