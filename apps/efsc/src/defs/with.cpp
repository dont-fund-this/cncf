#include "../type.hpp"
#include <vector>
#include <string_view>

size_t impl_count();
Defs impl_all();

namespace state {
    static std::vector<Def> defs;
    static bool did = false;
}

static bool same(const char *left, const char *right) {
    if (!left || !right) return false;
    return std::string_view(left) == std::string_view(right);
}

static void load() {
    if (!state::did) {
        size_t count = impl_count();
        Defs builtins = impl_all();
        state::defs.assign(builtins, builtins + count);
        state::did = true;
    }
}

Defs with() {
    load();
    return state::defs.data();
}

size_t with_count() {
    load();
    return state::defs.size();
}

int more(Def def) {
    load();
    state::defs.insert(state::defs.begin() + impl_count(), def);
    return 0;
}

int less(Def def) {
    load();
    for (auto it = state::defs.begin() + impl_count(); it != state::defs.end(); ++it) {
        if (same(it->sid, def.sid) || same(it->tag, def.tag)) {
            state::defs.erase(it);
            return 0;
        }
    }
    return -1;
}
