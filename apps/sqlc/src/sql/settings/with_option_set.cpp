#include "fit_option_set.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::settings {

def with_option_set() {
    return {
        "sql.option-set",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_option_set(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string key, value;
            try {
                key   = p.is_object() ? p.value("key", std::string{})   : std::string{};
                value = p.is_object() ? p.value("value", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            if (!key.empty()) sql::state().options[key] = value;
            send_to(options, json{{"ok", !key.empty()}, {"key", key}, {"value", value}}, {});
            return 1;
        },
    };
}

}
