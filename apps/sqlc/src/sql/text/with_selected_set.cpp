#include "fit_selected_set.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::text {

def with_selected_set() {
    return {
        "sql.selected-text-set",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_set(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string text;
            try { text = p.is_object() ? p.value("text", std::string{}) : std::string{}; } catch (const std::exception&) {}

            sql::state().selected_text = text;
            send_to(options, json{{"ok", true}, {"text", text}}, {});
            return 1;
        },
    };
}

}
