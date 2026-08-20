#include "state.h"
#include "fit_list.h"
#include "parse_payload.h"
#include "list_under.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>
#include <string>

Def with_list() {
    return {
        "efs.list",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_list(address); },
        [](Address address, Payload payload, Options options) -> int {
auto reply_err = [&](const std::string& msg) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", msg}}, {});
                return true;
            };

            std::string err;
            const auto j = parse_payload(payload, err);
            if (!err.empty()) return reply_err(err);

            const std::string path = j.value("path", std::string{});
            send_to(options, nlohmann::json{
                {"ok", true},
                {"entries", list_under(path)}
            }, {});
            return 1;
        },
    };
}
