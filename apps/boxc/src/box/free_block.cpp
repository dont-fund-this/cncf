#include "block.h"

#include <cstdlib>

namespace box {

void free_block(BlockDevice* bs) {
    if (!bs) return;
    auto* bk = static_cast<BlockBacking*>(bs->opaque);
    if (bk) {
        std::free(bk->data);
        delete bk;
    }
    delete bs;
}

}
