#include "state.h"
#include "fit_peek.h"
#include "b64_encode.h"
#include "bytes.h"
#include "parse_payload.h"
#include "require_path.h"
#include "../../send_to.h"

#include <algorithm>
#include <nlohmann/json.hpp>

Def with_peek() {
    return {
        "efs.peek",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_peek(address); },
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

            int count = j.value("count", 512);
            if (count <= 0) count = 512;

            PoolReadLock lock(efs::state().pool_mutex);
            const BytesView v = bytes_of(path);
            if (!v.ok) return reply_err(v.error);

            const std::size_t n = std::min<std::size_t>(static_cast<std::size_t>(count), v.size);
            send_to(options, nlohmann::json{
                {"ok", true}, {"size", n}, {"data", b64_encode(v.data, n)}
            }, {});
            return 1;
        },
    };
}
