#include "state.h"
#include "fit_help.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>

Def with_help() {
    return {
        "efs.help",
        "help",
        [](Address address, Payload, Options) -> bool { return fit_help(address); },
        [](Address address, Payload payload, Options options) -> int {
send_to(options, nlohmann::json{{"ok", true}, {"lib", "efs"}}, {});
            return 1;
        },
    };
}
