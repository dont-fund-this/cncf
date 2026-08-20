#include "fit_help.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>

def with_help() {
    return {
        "sql.help",
        "help",
        [](Address address, Payload, Options) -> bool { return fit_help(address); },
        [](Address address, Payload payload, Options options) -> int {
send_to(options, nlohmann::json{{"ok", true}, {"lib", "sql"}}, {});
            return 1;
        },
    };
}
