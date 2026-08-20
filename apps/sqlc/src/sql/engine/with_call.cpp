#include "engine/fit_call.h"
#include "engine/build_exec_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_call() {
    return {
        "sql.call",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_call(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string sql, target;
            try {
                sql    = p.is_object() ? p.value("sql", std::string{})    : std::string{};
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
            } catch (const std::exception&) {}

            for (const auto& f : build_exec_frames(sql, target, false))
                send_to(options, f, {});
            return 1;
        },
    };
}
}
