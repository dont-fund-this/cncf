#include "engine/import/fit_import_virt_flags.h"
#include "engine/import/build_virtual_frames.h"
#include "engine/import/flags_of.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::engine {

def with_import_virt_flags() {
    return {
        "sql.import.virt.flags",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_import_virt_flags(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            const bool real_only = p.is_object() && (p.contains("batch") || p.contains("commit") || p.contains("skip"));
            if (real_only) {
                send_to(options, json{{"kind", "error"}, {"severity", "fatal"}, {"code", "flag"}, {"text", "batch/commit/skip apply only to the real import"}}, {});
                send_to(options, json{{"kind", "done"}, {"rc", 1}}, {});
                return true;
            }

            std::string target, table, file;
            Flags flags;
            try {
                target = p.is_object() ? p.value("target", std::string{}) : std::string{};
                table  = p.is_object() ? p.value("table", std::string{})  : std::string{};
                file   = p.is_object() ? p.value("file", std::string{})   : std::string{};
                flags  = flags_of(p);
            } catch (const std::exception&) {}

            for (const auto& f : build_virtual_frames(target, table, file, flags))
                send_to(options, f, {});
            return 1;
        },
    };
}
}
