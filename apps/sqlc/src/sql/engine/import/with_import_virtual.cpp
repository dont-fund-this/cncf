#include "engine/import/fit_import_virtual.h"
#include "engine/import/build_virtual_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_import_virtual() {
    return {
        "sql.import.virtual",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_import_virtual(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json reply{{"ok", false}};
            try {
                auto p = nlohmann::json::parse(payload ? payload : "{}");
                const std::string target = p.value("target", std::string{});
                const std::string table  = p.value("table", std::string{});
                const std::string file   = p.value("file", std::string{});
                reply = {{"ok", true}, {"frames", build_virtual_frames(target, table, file)}};
            } catch (const std::exception&) {}
            send_to(options, reply, {});
            return 1;
        },
    };
}
}
