#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <unistd.h>

namespace foss {

inline bool read_file_bytes(int fd, size_t expected, std::vector<uint8_t>& buf) {
    buf.resize(expected);
    size_t total = 0;
    while (total < expected) {
        ssize_t got = read(fd, buf.data() + total, expected - total);
        if (got <= 0) return false;
        total += (size_t)got;
    }
    return total == expected;
}

}
