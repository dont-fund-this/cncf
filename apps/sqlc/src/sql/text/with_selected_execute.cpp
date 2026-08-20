#include "fit_selected_execute.h"
#include "state.h"
#include "engine/build_exec_frames.h"
#include "impl/capture_results.h"
#include "with.h"
#include "send_to.h"

#include <nlohmann/json.hpp>
#include <string>

namespace sql::text {

def with_selected_execute() {
    return {
        "sql.selected-text-execute",
        "sql",
        [](Address address, Payload, Options) -> bool { return fit_selected_execute(address); },
        [](Address address, Payload payload, Options options) -> int {
using nlohmann::json;

            const json p = json::parse(payload ? payload : "{}", nullptr, false);
            std::string target;
            try { target = p.is_object() ? p.value("target", std::string{}) : std::string{}; } catch (const std::exception&) {}

            auto& st = sql::state();
            st.results.clear();
            st.selected_result = -1;
            st.history.push_back(st.selected_text);

            const json frames = build_exec_frames(st.selected_text, target, true);
            for (const auto& rs : capture_results(frames)) st.results.push_back(rs);
            for (const auto& f : frames) send_to(options, f, {});
            return 1;
        },
    };
}

}
