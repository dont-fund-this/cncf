#pragma once

#include <cstdint>
#include <cstdio>

namespace foss {

inline void w32(FILE* f, uint32_t v) {
    uint8_t b[4] = {(uint8_t)v, (uint8_t)(v >> 8), (uint8_t)(v >> 16), (uint8_t)(v >> 24)};
    fwrite(b, 1, 4, f);
}

}
