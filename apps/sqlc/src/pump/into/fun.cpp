#include "../../type.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

static thread_local std::string last_into;

Address get_into(Options options) {
    if (!options) return nullptr;

    auto j = nlohmann::json::parse(options, nullptr, false);
    if (j.is_object() && j.contains("into") && j["into"].is_string()) {
        last_into = j["into"].get<std::string>();
        return last_into.c_str();
    }

    std::string_view opt(options);
    if (!opt.empty() && opt != "once" && opt != "many" && opt != "none" && !opt.starts_with("{") && !opt.starts_with("[")) {
        return options;
    }
    return nullptr;
}

bool has_verb(Options options, const char* verb) {
    if (!options) return std::string_view(verb) == "GET";

    auto j = nlohmann::json::parse(options, nullptr, false);
    if (j.is_object() && j.contains("verb") && j["verb"].is_string()) {
        return j["verb"].get<std::string>() == verb;
    }

    return std::string_view(verb) == "GET";
}
