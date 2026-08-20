#include "engine/import/fit_ingest.h"
#include "engine/import/build_virtual_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_ingest() {
    return {
        "sql.ingest",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_ingest(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, table, file;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{})  : std::string{};
                file   = p.is_object() ? p.value("file", std::string{})   : std::string{};
            } catch (const std::exception&) {}

            for (const auto& f : build_virtual_frames(target, table, file))
                send_to(options, f, {});
            return 1;
        },
    };
}
}
