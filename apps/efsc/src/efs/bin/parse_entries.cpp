#include "bin/internals.h"

#include <cstring>

namespace bin {

bool parse_entries(const uint8_t* base, size_t bytes, uint32_t count,
                   std::vector<ParsedEntry>& out, std::string& err) {
    const uint8_t* ptr = base + 8;
    const uint8_t* end = base + bytes;
    out.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
        if (ptr + 20 > end) {
            err = "Truncated entry header at index " + std::to_string(i);
            return false;
        }
        uint32_t path_len = 0; std::memcpy(&path_len, ptr, 4); ptr += 4;
        uint64_t size     = 0; std::memcpy(&size,     ptr, 8); ptr += 8;
        uint64_t offset   = 0; std::memcpy(&offset,   ptr, 8); ptr += 8;

        if (path_len == 0 || ptr + path_len > end) {
            err = "Invalid path length " + std::to_string(path_len) + " at entry " + std::to_string(i);
            return false;
        }
        if (offset > bytes || size > bytes || offset + size > bytes) {
            err = "Entry " + std::to_string(i) + " references bytes ["
                + std::to_string(offset) + ", " + std::to_string(offset + size)
                + ") outside mapping of " + std::to_string(bytes);
            return false;
        }
        out.push_back({ std::string(reinterpret_cast<const char*>(ptr), path_len), offset, size });
        ptr += path_len;
    }
    return true;
}

}
