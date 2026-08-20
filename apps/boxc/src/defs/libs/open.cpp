#include "../../type.hpp"
#include <dlfcn.h>

void* lib_open(const char* path) {
    if (!path) return nullptr;
    return dlopen(path, RTLD_LAZY | RTLD_LOCAL);
}
