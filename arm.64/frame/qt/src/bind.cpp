#include "type.h"
#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

Cabi* bind(const std::string& binary_path) {
    std::string filename = fs::path(binary_path).filename().string();
    if (filename == "c" || filename == "cpp" || filename == "rust" ||
        filename == "go" || filename == "swift" || filename == "haskell" ||
        filename == "zig" || filename == "v") {
        return nullptr;
    }

    void* lib = dlopen(binary_path.c_str(), RTLD_LAZY);
    if (!lib) {
        return nullptr;
    }

    auto more = reinterpret_cast<MoreFn>(dlsym(lib, "More"));
    auto pump = reinterpret_cast<PumpFn>(dlsym(lib, "Pump"));
    auto less = reinterpret_cast<LessFn>(dlsym(lib, "Less"));

    if (!more || !pump || !less) {
        dlclose(lib);
        return nullptr;
    }

    auto* cabi = new Cabi();
    cabi->name = filename;
    cabi->path = binary_path;
    cabi->lib  = lib;
    cabi->More = more;
    cabi->Pump = pump;
    cabi->Less = less;

    return cabi;
}
