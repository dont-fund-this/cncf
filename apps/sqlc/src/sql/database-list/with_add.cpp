#include "fit_add.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::database_list {

def with_add() {
    return {
        "sql.database-add",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_add(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string name, target, cohort;
            try {
                name   = p.is_object() ? p.value("name", std::string{})   : std::string{};
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                cohort = p.is_object() ? p.value("cohort", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            if (name.empty()) { send_to(options, json{{"ok", false}, {"error", "name required"}}, {}); return true; }

            auto& quiver = sql::state().quiver;
            json kept = json::array();
            for (const auto& e : quiver) if (e.value("name", std::string{}) != name) kept.push_back(e);
            kept.push_back(json{{"name", name}, {"target", target}, {"cohort", cohort.empty() ? std::string("user") : cohort}});
            quiver = kept;
            send_to(options, json{{"ok", true}, {"name", name}, {"target", target}}, {});
            return 1;
        },
    };
}

}
