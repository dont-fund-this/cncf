#include "../type.hpp"
#include "../box/state.h"

static const Def ALL_DEFS[] = {
    BoxStart,
    BoxStop,
    BoxSend,
    BoxPoll,
};

size_t impl_count() {
    return 4;
}

Defs impl_all() {
    return ALL_DEFS;
}
