#pragma once

#include "entry.h"
#include "w16.h"
#include "w32.h"

#include <cstdint>
#include <cstdio>
#include <vector>

namespace foss {

inline void write_local_header(FILE* out, const Entry& e, const std::vector<uint8_t>& payload) {
    w32(out, 0x04034b50);
    w16(out, 20);
    w16(out, 0);
    w16(out, e.method);
    w16(out, DOS_TIME);
    w16(out, DOS_DATE);
    w32(out, e.crc);
    w32(out, e.comp_size);
    w32(out, e.uncomp_size);
    w16(out, (uint16_t)e.name.size());
    w16(out, 0);
    fwrite(e.name.data(), 1, e.name.size(), out);
    if (!payload.empty()) fwrite(payload.data(), 1, payload.size(), out);
}

}
