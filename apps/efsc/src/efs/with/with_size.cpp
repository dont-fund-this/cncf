#include "state.h"
#include "fit_size.h"
#include "parse_path.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>

Def with_size() {
    return {
        "efs.size",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_size(address); },
        [](Address address, Payload payload, Options options) -> int {
std::string err;
            const std::string path = parse_path(payload, err);
            if (path.empty()) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", err}}, {});
                return true;
            }

            auto& s = efs::state();
            PoolReadLock lock(s.pool_mutex);
            auto it = s.pool.index.find(path);
            if (it == s.pool.index.end()) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", "not found"}}, {});
                return true;
            }

            send_to(options, nlohmann::json{{"ok", true}, {"size", it->second.size}}, {});
            return 1;
        },
    };
}
