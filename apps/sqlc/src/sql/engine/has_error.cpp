#include "engine/has_error.h"

#include <string>

bool has_error(const nlohmann::json& frames) {
    for (const auto& f : frames) if (f.value("kind", std::string{}) == "error") return true;
    return false;
}
