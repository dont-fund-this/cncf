#include "state.h"

namespace sql {

auto state() -> State& {
    static State s;
    return s;
}

}
