#include "engine/parse/fit_parse.h"
#include "engine/parse/statements.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_parse() {
    return {
        "sql.parse",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_parse(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json reply{{"ok", false}};
            try {
                auto p = nlohmann::json::parse(payload ? payload : "{}");
                const std::string sql = p.value("sql", std::string{});
                reply = {{"ok", true}, {"statements", statements(sql)}};
            } catch (const std::exception&) {}
            send_to(options, reply, {});
            return 1;
        },
    };
}
}
