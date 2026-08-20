#pragma once

#include "entry.h"
#include "build_entry.h"
#include "write_local_header.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace foss {

inline void append_entry(FILE* out, const std::string& line, int level,
                         std::vector<Entry>& entries) {
    Entry e{};
    std::vector<uint8_t> payload;
    if (!build_entry(line, level, e, payload)) return;
    e.local_offset = (uint32_t)ftell(out);
    write_local_header(out, e, payload);
    entries.push_back(std::move(e));
}

}
