#include "type.h"
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

std::vector<std::string> find(const std::string& target_dir) {
    std::string dir = target_dir;
    if (dir.empty()) {
        if (const char* env_dir = std::getenv("DIST_DIR"); env_dir && *env_dir) {
            dir = env_dir;
        } else {
            std::vector<std::string> candidates = {
                (fs::current_path() / "dist").string(),
                (fs::current_path() / "../../dist").string(),
                (fs::current_path() / "../../../dist").string(),
                "dist"
            };
            for (const auto& c : candidates) {
                if (fs::exists(c) && fs::is_directory(c)) {
                    dir = c;
                    break;
                }
            }
        }
    }
    if (dir.empty() || !fs::exists(dir)) return {};

    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().filename() != ".DS_Store") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}
