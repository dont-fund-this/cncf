#include "capture_results.h"

#include <string>

nlohmann::json capture_results(const nlohmann::json& frames) {
    using nlohmann::json;
    json out = json::array();
    json cur;
    for (const auto& f : frames) {
        const std::string k = f.value("kind", std::string{});
        if (k == "resultset") cur = json{{"columns", f.value("columns", json::array())}, {"rows", json::array()}};
        if (k == "rows" && cur.is_object()) { cur["rows"] = f.value("rows", json::array()); out.push_back(cur); cur = json(); }
    }
    return out;
}
