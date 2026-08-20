#include "../../type.hpp"
#include <nlohmann/json.hpp>
#include <string_view>

namespace want {
    bool none(Options options) {
        if (!options) return false;
        std::string_view opt(options);
        if (opt == "none") return true;

        auto j = nlohmann::json::parse(options, nullptr, false);
        if (j.is_object()) {
            if (j.value("none", false)) return true;
            if (j.value("once", false) || j.value("many", false)) return false;
        }
        return false;
    }
}
