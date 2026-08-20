#pragma once

#include "file_entry.h"

#include <fstream>
#include <ios>

namespace pack {

inline void copy_file(const FileEntry& e, std::ofstream& out) {
    std::ifstream in(e.full_path, std::ios::binary);
    if (!in) return;
    char buffer[8192];
    while (true) {
        in.read(buffer, sizeof(buffer));
        const std::streamsize bytes_read = in.gcount();
        if (bytes_read > 0) out.write(buffer, bytes_read);
        if (bytes_read > 0 && out.fail()) return;
        if (!in) return;
    }
}

}
