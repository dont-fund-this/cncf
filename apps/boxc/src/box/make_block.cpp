#include "block.h"
#include "state.h"

namespace box {

BlockDevice* make_block(const std::string& path) {
    long len = 0;
    unsigned char* buf = read_file(path, &len);
    if (!buf) return nullptr;
    auto* bk = new BlockBacking{buf, static_cast<int64_t>(len)};
    auto* bs = new BlockDevice{};
    bs->opaque           = bk;
    bs->get_sector_count = block_count;
    bs->read_async       = block_read;
    bs->write_async      = block_write;
    return bs;
}

}
