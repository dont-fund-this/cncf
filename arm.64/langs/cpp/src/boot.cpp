#include "type.h"
#include <cstdlib>

std::vector<std::string> find(const std::string& target_dir);
Cabi* bind(const std::string& binary_path);

std::vector<Cabi*> boot(const std::string& target_dir) {
    std::vector<Cabi*> engines;
    if (const char* env_lib = std::getenv("PAT_LIB"); env_lib && *env_lib) {
        if (auto* c = bind(env_lib)) {
            engines.push_back(c);
            return engines;
        }
    }

    auto files = find(target_dir);
    for (const auto& file : files) {
        if (auto* c = bind(file)) {
            engines.push_back(c);
        }
    }
    return engines;
}
