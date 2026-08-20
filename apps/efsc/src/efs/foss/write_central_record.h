#pragma once

#include "entry.h"
#include "w16.h"
#include "w32.h"

#include <cstdint>
#include <cstdio>

namespace foss {

inline void write_central_record(FILE* out, const Entry& e) {
    w32(out, 0x02014b50);
    w16(out, 20);
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
    w16(out, 0);
    w16(out, 0);
    w16(out, 0);
    w32(out, 0);
    w32(out, e.local_offset);
    fwrite(e.name.data(), 1, e.name.size(), out);
}

}
