#include "fit_add.h"
#include "state.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::execute::history {

def with_add() {
    return {
        "sql.execute-history-add",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_add(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string text;
            try { text = p.is_object() ? p.value("sql", std::string{}) : std::string{}; } catch (const std::exception&) {}

            auto& h = sql::state().history;
            if (!text.empty()) h.push_back(text);
            send_to(options, json{{"ok", !text.empty()}, {"count", static_cast<int>(h.size())}}, {});
            return 1;
        },
    };
}

}
