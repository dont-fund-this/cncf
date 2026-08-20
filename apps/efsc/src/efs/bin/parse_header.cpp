#include "bin/internals.h"

#include <cstring>

namespace bin {

bool parse_header(const uint8_t* base, size_t bytes, uint32_t& count, std::string& err) {
    if (bytes < 8) { err = "Truncated bin (header < 8 bytes)"; return false; }
    if (std::memcmp(base, "PATB", 4) != 0) { err = "Invalid magic"; return false; }

    std::memcpy(&count, base + 4, 4);

    const uint64_t max_possible = (static_cast<uint64_t>(bytes) - 8) / 20;
    if (count > max_possible) {
        err = "Bin header claims " + std::to_string(count)
            + " entries but file can hold at most " + std::to_string(max_possible);
        return false;
    }
    return true;
}

}
