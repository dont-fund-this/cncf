#include "state.h"

namespace efs {

auto state() -> State& {
    static State s;
    return s;
}

}
