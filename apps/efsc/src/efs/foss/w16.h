#pragma once

#include <cstdint>
#include <cstdio>

namespace foss {

inline void w16(FILE* f, uint16_t v) {
    uint8_t b[2] = {(uint8_t)v, (uint8_t)(v >> 8)};
    fwrite(b, 1, 2, f);
}

}
