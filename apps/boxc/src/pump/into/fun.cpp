#include "../../type.hpp"
#include <cstring>
#include <string_view>

Address get_into(Options options) {
    if (!options) return nullptr;
    std::string_view opt(options);
    auto pos = opt.find("into:");
    if (pos != std::string_view::npos) {
        return options + pos + 5;
    }
    if (opt.rfind("{", 0) == 0 || opt.rfind("[", 0) == 0) {
        return nullptr;
    }
    if (!opt.empty() && opt != "once" && opt != "many" && opt != "none") {
        return options;
    }
    return nullptr;
}

bool has_verb(Options options, const char* verb) {
    if (!options) return std::string_view(verb) == "GET";
    std::string_view opt(options);
    auto pos = opt.find("verb:");
    if (pos != std::string_view::npos) {
        auto rest = opt.substr(pos + 5);
        auto comma = rest.find(',');
        if (comma != std::string_view::npos) rest = rest.substr(0, comma);
        return rest == verb;
    }
    return std::string_view(verb) == "GET";
}
