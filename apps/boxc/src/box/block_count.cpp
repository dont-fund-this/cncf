#include "block.h"

namespace box {

int64_t block_count(BlockDevice* bs) {
    auto* bk = static_cast<BlockBacking*>(bs->opaque);
    return bk->size / 512;
}

}
