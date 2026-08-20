#include "engine/import/fit_stream.h"
#include "engine/import/build_import_frames.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_stream() {
    return {
        "sql.stream",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_stream(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target, table, file;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{})  : std::string{};
                file   = p.is_object() ? p.value("file", std::string{})   : std::string{};
            } catch (const std::exception&) {}

            const json frames = build_import_frames(target, table, file, [options](long n) {
                send_to(options, nlohmann::json{{"kind", "progress"}, {"rows", n}}, {});
            });
            for (const auto& f : frames)
                send_to(options, f, {});
            return 1;
        },
    };
}
}
