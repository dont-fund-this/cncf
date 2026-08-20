#include "engine/fit_query.h"
#include "engine/build_exec_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_query() {
    return {
        "sql.query",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_query(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json reply{{"ok", false}};
            try {
                auto p = nlohmann::json::parse(payload ? payload : "{}");
                const std::string sql    = p.value("sql", std::string{});
                const std::string target = p.value("target", std::string{});
                reply = {{"ok", true}, {"frames", build_exec_frames(sql, target, false)}};
            } catch (const std::exception&) {}
            send_to(options, reply, {});
            return 1;
        },
    };
}
}
