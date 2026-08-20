#include "state.h"
#include "fit_sniff.h"
#include "sniff.h"
#include "bytes.h"
#include "parse_payload.h"
#include "require_path.h"
#include "../../send_to.h"

#include <algorithm>
#include <nlohmann/json.hpp>

Def with_sniff() {
    return {
        "efs.sniff",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_sniff(address); },
        [](Address address, Payload payload, Options options) -> int {
auto reply_err = [&](const std::string& msg) {
                send_to(options, nlohmann::json{{"ok", false}, {"error", msg}}, {});
                return true;
            };

            std::string err;
            const auto j = parse_payload(payload, err);
            if (!err.empty()) return reply_err(err);

            const std::string path = require_path(j, err);
            if (path.empty()) return reply_err(err);

            PoolReadLock lock(efs::state().pool_mutex);
            const BytesView v = bytes_of(path);
            if (!v.ok) return reply_err(v.error);

            const std::size_t n = std::min<std::size_t>(512, v.size);
            const std::string kind = sniff(reinterpret_cast<const unsigned char*>(v.data), n);
            send_to(options, nlohmann::json{
                {"ok", true}, {"kind", kind}, {"size", v.size}
            }, {});
            return 1;
        },
    };
}
