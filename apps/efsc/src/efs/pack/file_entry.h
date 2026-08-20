#pragma once

#include <cstdint>
#include <string>

namespace pack {

struct FileEntry {
    std::string rel_path;
    std::string full_path;
    uint64_t    size;
    uint64_t    offset;
};

}
