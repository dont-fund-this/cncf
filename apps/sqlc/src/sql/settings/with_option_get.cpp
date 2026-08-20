#include "fit_option_get.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::settings {

def with_option_get() {
    return {
        "sql.option-get",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_option_get(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string key;
            try { key = p.is_object() ? p.value("key", std::string{}) : std::string{}; } catch (const std::exception&) {}

            const auto& opts = sql::state().options;
            const auto it = opts.find(key);
            send_to(options, json{{"key", key}, {"value", it != opts.end() ? json(it->second) : json(nullptr)}}, {});
            return 1;
        },
    };
}

}
