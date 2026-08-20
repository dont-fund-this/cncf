#include "type.h"
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

std::vector<std::string> find(const std::string& target_dir) {
    std::string dir = target_dir;
    if (dir.empty()) {
        const char* env_dir = std::getenv("DIST_DIR");
        if (env_dir && *env_dir) {
            dir = env_dir;
        } else {
            std::vector<std::string> candidates = {
                (fs::current_path() / "dist").string(),
                (fs::current_path() / "../../dist").string(),
                (fs::current_path() / "../../../dist").string(),
                "dist"
            };
            dir = "dist";
            for (const auto& c : candidates) {
                if (fs::exists(c) && fs::is_directory(c)) {
                    dir = fs::canonical(c).string();
                    break;
                }
            }
        }
    }

    std::vector<std::string> files;
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        return files;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().filename() != ".DS_Store") {
            files.push_back(entry.path().string());
        }
    }

    return files;
}
