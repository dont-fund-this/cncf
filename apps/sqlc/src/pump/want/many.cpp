#include "../../type.hpp"
#include <nlohmann/json.hpp>
#include <string_view>

namespace want {
    bool many(Options options) {
        if (!options) return false;
        std::string_view opt(options);
        if (opt == "many") return true;

        auto j = nlohmann::json::parse(options, nullptr, false);
        if (j.is_object()) {
            return j.value("many", false);
        }
        return false;
    }
}
