#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <zlib.h>

namespace foss {

inline int compress_buffer(const uint8_t* src, size_t n,
                           std::vector<uint8_t>& dst, int level) {
    if (level == 0 || n == 0) {
        dst.assign(src, src + n);
        return 0;
    }
    z_stream zs = {};
    if (deflateInit2(&zs, level, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        return -1;
    dst.resize(n + (n / 1000) + 64);
    zs.next_in   = const_cast<Bytef*>(src);
    zs.avail_in  = (uInt)n;
    zs.next_out  = dst.data();
    zs.avail_out = (uInt)dst.size();
    int ret = deflate(&zs, Z_FINISH);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) return -1;
    dst.resize(zs.total_out);
    if (dst.size() >= n) {
        dst.assign(src, src + n);
        return 0;
    }
    return 8;
}

}
