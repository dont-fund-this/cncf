#include "state.h"
#include "fit_load.h"
#include "bin.h"
#include "parse_path.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>

Def with_load() {
    return {
        "efs.load",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_load(address); },
        [](Address address, Payload payload, Options options) -> int {
            auto reply_err = [&](const std::string& msg) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", msg}}, {});
                return 1;
            };
            auto reply_ok = [&]() {
                send_to(options, nlohmann::json{{"ok", true}}, {});
                return 1;
            };

            std::string err;
            const std::string path = parse_path(payload, err);
            if (path.empty()) return reply_err(err);

            auto& s = efs::state();
            PoolWriteLock lock(s.pool_mutex);

            if (s.pool.index.find(path) != s.pool.index.end()) return reply_ok();

            for (const auto& f : s.pool.pool) {
                if (f.path == path) return reply_ok();
            }

            if (!load_bin(path, err)) return reply_err(err);
            return reply_ok();
        },
    };
}
