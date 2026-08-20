#include "fit_selected_set.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_list {

def with_selected_set() {
    return {
        "sql.selected-database-set",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_set(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string name;
            try { name = p.is_object() ? p.value("name", std::string{}) : std::string{}; } catch (const std::exception&) {}

            auto& st = sql::state();
            bool present = false;
            for (const auto& e : st.quiver) if (e.value("name", std::string{}) == name) { present = true; break; }
            if (!present) { send_to(options, json{{"ok", false}, {"error", "not in list: " + name}}, {}); return true; }
            st.selected = name;
            st.selected_object.clear();
            st.selected_object_type.clear();
            send_to(options, json{{"ok", true}, {"selected", name}}, {});
            return 1;
        },
    };
}

}
