#include "engine/import/fit_import.h"
#include "engine/import/build_import_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_import() {
    return {
        "sql.import",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_import(address); },
        [](Address address, Payload payload, Options options) -> int {
nlohmann::json reply{{"ok", false}};
            try {
                auto p = nlohmann::json::parse(payload ? payload : "{}");
                const std::string target = p.value("target", std::string{});
                const std::string table  = p.value("table", std::string{});
                const std::string file   = p.value("file", std::string{});
                reply = {{"ok", true}, {"frames", build_import_frames(target, table, file)}};
            } catch (const std::exception&) {}
            send_to(options, reply, {});
            return 1;
        },
    };
}
}
