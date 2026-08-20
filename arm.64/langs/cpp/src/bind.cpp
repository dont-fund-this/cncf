#include "type.h"
#include <dlfcn.h>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

Cabi* bind(const std::string& binary_path) {
    std::string filename = fs::path(binary_path).filename().string();
    std::vector<std::string> skips = {"c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample"};
    if (std::find(skips.begin(), skips.end(), filename) != skips.end()) {
        return nullptr;
    }

    void* handle = dlopen(binary_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!handle) return nullptr;

    auto more = reinterpret_cast<MoreFn>(dlsym(handle, "More"));
    auto pump = reinterpret_cast<PumpFn>(dlsym(handle, "Pump"));
    auto less = reinterpret_cast<LessFn>(dlsym(handle, "Less"));

    if (!pump) {
        dlclose(handle);
        return nullptr;
    }

    auto* cabi = new Cabi();
    cabi->name = filename;
    cabi->path = binary_path;
    cabi->handle = handle;
    cabi->More = more;
    cabi->Pump = pump;
    cabi->Less = less;
    return cabi;
}
