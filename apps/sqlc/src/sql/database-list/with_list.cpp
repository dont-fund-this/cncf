#include "fit_list.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_list {

def with_list() {
    return {
        "sql.database-list",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_list(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string cohort;
            try { cohort = p.is_object() ? p.value("cohort", std::string{}) : std::string{}; } catch (const std::exception&) {}

            json out = json::array();
            for (const auto& e : sql::state().quiver)
                if (cohort.empty() || e.value("cohort", std::string{}) == cohort) out.push_back(e);
            send_to(options, json{{"dbs", out}}, {});
            return 1;
        },
    };
}

}
