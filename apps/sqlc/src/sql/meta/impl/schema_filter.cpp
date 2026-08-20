#include "schema_filter.h"
#include "engine/lower.h"
#include "engine/squote.h"

#include <string>

std::string schema_filter(const nlohmann::json& filters) {
    std::string out;
    if (!filters.is_array()) return out;
    for (const auto& f : filters) {
        if (!f.is_object() || lower(f.value("property", std::string{})) != "name") continue;
        const std::string op = lower(f.value("operator", std::string{}));
        const std::string v = squote(f.value("value", std::string{}));
        if (op == "contains")         out += " AND name LIKE '%" + v + "%'";
        if (op == "equals")           out += " AND name = '" + v + "'";
        if (op == "does not contain") out += " AND name NOT LIKE '%" + v + "%'";
    }
    return out;
}
