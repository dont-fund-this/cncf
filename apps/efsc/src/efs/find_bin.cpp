#include "find_bin.h"

#include <dlfcn.h>
#include <sys/stat.h>

std::string find_bin() {
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(&find_bin), &info) && info.dli_fname) {
        std::string self = info.dli_fname;
        auto slash = self.find_last_of('/');
        std::string dir = (slash == std::string::npos) ? "." : self.substr(0, slash);
        std::string cand = dir + "/libefs.bin";
        struct stat st;
        if (::stat(cand.c_str(), &st) == 0) return cand;
    }
    static const char* fallbacks[] = {
        "libefs.bin",
        "dist/lib/libefs.bin",
        "../dist/lib/libefs.bin",
        "../../dist/lib/libefs.bin",
    };
    for (const char* p : fallbacks) {
        struct stat st;
        if (::stat(p, &st) == 0) return p;
    }
    return {};
}
