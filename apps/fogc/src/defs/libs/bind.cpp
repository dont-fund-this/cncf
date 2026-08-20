#include "../../type.hpp"
#include <dlfcn.h>

void* lib_bind(void* handle, const char* symbol) {
    if (!handle || !symbol) return nullptr;
    return dlsym(handle, symbol);
}
