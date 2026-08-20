#include "../type.hpp"

int less(Def def);

extern "C" int Less(Def def) {
    return less(def);
}
