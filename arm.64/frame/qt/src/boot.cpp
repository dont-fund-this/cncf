#include "type.h"
#include <cstdlib>

extern std::vector<std::string> find(const std::string& target_dir);
extern Cabi* bind(const std::string& binary_path);

std::vector<Cabi*> boot(const std::string& target_dir) {
    std::vector<Cabi*> engines;

    const char* env_lib = std::getenv("PAT_LIB");
    if (env_lib && *env_lib) {
        Cabi* bound = bind(env_lib);
        if (bound) {
            engines.push_back(bound);
            return engines;
        }
    }

    std::vector<std::string> files = find(target_dir);
    for (const auto& file : files) {
        Cabi* bound = bind(file);
        if (bound) {
            engines.push_back(bound);
        }
    }

    return engines;
}
