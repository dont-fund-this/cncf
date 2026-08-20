#pragma once

namespace foss {

inline bool is_level_flag(const char* a) {
    return a[0] == '-' && a[1] >= '0' && a[1] <= '9' && a[2] == 0;
}

}
