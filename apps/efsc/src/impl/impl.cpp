#include "../type.hpp"
#include "../efs/state.h"
#include <vector>

static std::vector<Def> ALL_DEFS;

static void ensure_defs() {
    if (ALL_DEFS.empty()) {
        ALL_DEFS = efs_with();
    }
}

size_t impl_count() {
    ensure_defs();
    return ALL_DEFS.size();
}

Defs impl_all() {
    ensure_defs();
    return ALL_DEFS.data();
}
