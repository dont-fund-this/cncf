#pragma once

#include "file_entry.h"
#include "copy_file.h"

#include <fstream>
#include <vector>

namespace pack {

inline void write_blobs(std::ofstream& out, const std::vector<FileEntry>& entries) {
    for (const auto& e : entries) copy_file(e, out);
}

}
