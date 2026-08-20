#include "engine/deploy/fit_deploy.h"
#include "engine/deploy/build_deploy_reply.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_deploy() {
    return {
        "sql.deploy",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_deploy(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json reply{{"ok", false}};
            try {
                auto p = nlohmann::json::parse(payload ? payload : "{}");
                reply = build_deploy_reply(p.value("db", std::string{}));
            } catch (const std::exception&) {}
            send_to(options, reply, {});
            return 1;
        },
    };
}
}
