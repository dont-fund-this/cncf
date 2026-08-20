#pragma once

#include "entry.h"
#include "compress_buffer.h"
#include "read_file_bytes.h"

#include <cstdint>
#include <cstddef>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <zlib.h>

namespace foss {

inline bool build_entry(const std::string& name, int level,
                        Entry& e, std::vector<uint8_t>& payload) {
    int fd = open(name.c_str(), O_RDONLY);
    if (fd < 0) return false;
    struct stat st;
    if (fstat(fd, &st) < 0 || !S_ISREG(st.st_mode)) { close(fd); return false; }

    e.name = name;
    e.uncomp_size = (uint32_t)st.st_size;
    if (st.st_size == 0) {
        e.crc = 0; e.comp_size = 0; e.method = 0;
        close(fd);
        return true;
    }
    std::vector<uint8_t> buf;
    if (!read_file_bytes(fd, (size_t)st.st_size, buf)) { close(fd); return false; }
    close(fd);

    e.crc = crc32(0, buf.data(), (uInt)st.st_size);
    int m = compress_buffer(buf.data(), (size_t)st.st_size, payload, level);
    if (m < 0) return false;
    e.method = (uint16_t)m;
    e.comp_size = (uint32_t)payload.size();
    return true;
}

}
