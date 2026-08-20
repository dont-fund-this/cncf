#include "../type.hpp"
#include "../sql/with.h"
#include <vector>

static std::vector<Def> ALL_DEFS;

static void ensure_defs() {
    if (ALL_DEFS.empty()) {
        ALL_DEFS = sql_with();
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
