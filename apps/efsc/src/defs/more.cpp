#include "../type.hpp"

int more(Def def);

extern "C" int More(Def def) {
    return more(def);
}
