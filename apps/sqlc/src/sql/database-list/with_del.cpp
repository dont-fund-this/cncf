#include "fit_del.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_list {

def with_del() {
    return {
        "sql.database-del",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_del(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string name;
            try { name = p.is_object() ? p.value("name", std::string{}) : std::string{}; } catch (const std::exception&) {}

            auto& st = sql::state();
            json kept = json::array();
            for (const auto& e : st.quiver) if (e.value("name", std::string{}) != name) kept.push_back(e);
            st.quiver = kept;
            if (st.selected == name) { st.selected.clear(); st.selected_object.clear(); st.selected_object_type.clear(); }
            send_to(options, json{{"ok", true}}, {});
            return 1;
        },
    };
}

}
