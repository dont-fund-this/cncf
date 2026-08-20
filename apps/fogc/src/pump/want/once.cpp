#include "../../type.hpp"
#include <nlohmann/json.hpp>
#include <string_view>

namespace want {
    bool once(Options options) {
        if (!options) return true;
        std::string_view opt(options);
        if (opt.empty() || opt == "once") return true;

        auto j = nlohmann::json::parse(options, nullptr, false);
        if (j.is_object()) {
            if (j.value("many", false)) return false;
            if (j.value("none", false)) return false;
            return true;
        }
        return opt != "none" && opt != "many";
    }
}
