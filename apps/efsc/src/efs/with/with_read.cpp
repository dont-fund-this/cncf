#include "state.h"
#include "fit_read.h"
#include "b64_encode.h"
#include "bytes.h"
#include "parse_path.h"
#include "../../send_to.h"

#include <nlohmann/json.hpp>

Def with_read() {
    return {
        "efs.read",
        "efs",
        [](Address address, Payload, Options) -> bool { return fit_read(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json retPayload = nlohmann::json::object();

            std::string err;
            const std::string path = parse_path(payload, err);
            if (path.empty()) {
                retPayload["ok"]    = false;
                retPayload["error"] = err;
                send_to(options, retPayload, {});
                return true;
            }

            PoolReadLock lock(efs::state().pool_mutex);
            const BytesView v = bytes_of(path);
            if (!v.ok) {
                retPayload["ok"]    = false;
                retPayload["error"] = v.error;
                send_to(options, retPayload, {});
                return true;
            }

            retPayload["ok"]   = true;
            retPayload["size"] = v.size;
            retPayload["data"] = b64_encode(v.data, v.size);
            send_to(options, retPayload, {});
            return 1;
        },
    };
}
