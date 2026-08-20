#include "block.h"

#include <cstring>

namespace box {

int block_read(BlockDevice* bs, uint64_t sector, uint8_t* buf, int n,
               BlockDeviceCompletionFunc*, void*) {
    auto* bk = static_cast<BlockBacking*>(bs->opaque);
    const int64_t off = static_cast<int64_t>(sector) * 512;
    const int64_t len = static_cast<int64_t>(n) * 512;
    if (off < 0 || len < 0 || off + len > bk->size) return -1;
    std::memcpy(buf, bk->data + off, static_cast<std::size_t>(len));
    return 0;
}

}
