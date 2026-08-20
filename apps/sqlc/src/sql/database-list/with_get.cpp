#include "fit_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_list {

def with_get() {
    return {
        "sql.database-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string name;
            try { name = p.is_object() ? p.value("name", std::string{}) : std::string{}; } catch (const std::exception&) {}

            for (const auto& e : sql::state().quiver)
                if (e.value("name", std::string{}) == name) { send_to(options, e, {}); return true; }
            send_to(options, json{{"name", name}, {"target", nullptr}}, {});
            return 1;
        },
    };
}

}
