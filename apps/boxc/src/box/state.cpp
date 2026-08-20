#include "state.h"

namespace box {

auto state() -> State& {
    static State s;
    return s;
}

}
