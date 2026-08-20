#pragma once

#include "file_entry.h"

#include <cstdint>
#include <fstream>
#include <vector>

namespace pack {

inline void write_header(std::ofstream& out, const std::vector<FileEntry>& entries) {
    const char magic[4] = {'P', 'A', 'T', 'B'};
    uint32_t   count    = static_cast<uint32_t>(entries.size());
    out.write(magic, 4);
    out.write(reinterpret_cast<const char*>(&count), 4);
    for (const auto& e : entries) {
        uint32_t path_len = static_cast<uint32_t>(e.rel_path.size());
        uint64_t size     = e.size;
        uint64_t offset   = e.offset;
        out.write(reinterpret_cast<const char*>(&path_len), 4);
        out.write(reinterpret_cast<const char*>(&size),     8);
        out.write(reinterpret_cast<const char*>(&offset),   8);
        out.write(e.rel_path.c_str(), path_len);
    }
}

}
