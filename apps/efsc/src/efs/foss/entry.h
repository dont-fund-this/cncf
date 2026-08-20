#pragma once

#include <cstdint>
#include <string>

namespace foss {

struct Entry {
    std::string name;
    uint32_t crc;
    uint32_t comp_size;
    uint32_t uncomp_size;
    uint32_t local_offset;
    uint16_t method;
};

inline constexpr uint16_t DOS_TIME = 0;
inline constexpr uint16_t DOS_DATE = 0x0021;

}
