#pragma once

#include "../type.hpp"
#include "pool.h"
#include <vector>
#include <shared_mutex>

namespace efs {

struct State {
    FilePool          pool;
    std::shared_mutex pool_mutex;
};

auto state() -> State&;

}

std::vector<Def> efs_with();
