#pragma once

#include "file_entry.h"

#include <cstdint>
#include <vector>

namespace pack {

inline void assign_offsets(std::vector<FileEntry>& entries) {
    uint64_t off = 4 + 4;
    for (const auto& e : entries) off += 4 + 8 + 8 + e.rel_path.size();
    for (auto& e : entries) { e.offset = off; off += e.size; }
}

}
