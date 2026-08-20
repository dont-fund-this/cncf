#pragma once

#include "w16.h"
#include "w32.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace foss {

inline void write_end_record(FILE* out, size_t count, uint32_t cd_size, uint32_t cd_off) {
    w32(out, 0x06054b50);
    w16(out, 0);
    w16(out, 0);
    w16(out, (uint16_t)count);
    w16(out, (uint16_t)count);
    w32(out, cd_size);
    w32(out, cd_off);
    w16(out, 0);
}

}
