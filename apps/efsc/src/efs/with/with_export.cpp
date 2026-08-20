#include "state.h"
#include "fit_export.h"
#include "parse_path.h"
#include "write_to_temp.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <string>

Def with_export() {
    return {
        "efs.export",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_export(address); },
        [](Address address, Payload payload, Options options) -> int {
auto reply_err = [&](const std::string& msg) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", msg}}, {});
                return true;
            };

            std::string err;
            const std::string path = parse_path(payload, err);
            if (!err.empty()) return reply_err(err);

            auto& s = efs::state();
            PoolReadLock lock(s.pool_mutex);
            auto it = s.pool.index.find(path);
            if (it == s.pool.index.end()) return reply_err("not found: " + path);

            const int idx = it->second.file_index;
            if (idx < 0 || idx >= static_cast<int>(s.pool.pool.size()))
                return reply_err("invalid pool index");

            const uint64_t offset = it->second.offset;
            const uint64_t size   = it->second.size;
            const char* start = static_cast<const char*>(s.pool.pool[idx].base) + offset;

            auto out_path = write_to_temp(path, start, size, err);
            if (out_path.empty()) return reply_err(err);

            send_to(options, nlohmann::json{
                {"ok",   true},
                {"path", out_path.string()},
                {"size", size}
            }, {});
            return 1;
        },
    };
}
