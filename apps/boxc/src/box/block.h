#pragma once

#include "emu.h"

#include <string>

namespace box {

struct BlockBacking {
    uint8_t* data;
    int64_t  size;
};

BlockDevice* make_block(const std::string& path);
void         free_block(BlockDevice* bs);
int64_t      block_count(BlockDevice* bs);
int          block_read(BlockDevice* bs, uint64_t sector, uint8_t* buf, int n,
                        BlockDeviceCompletionFunc* cb, void* opaque);
int          block_write(BlockDevice* bs, uint64_t sector, const uint8_t* buf, int n,
                         BlockDeviceCompletionFunc* cb, void* opaque);

}
