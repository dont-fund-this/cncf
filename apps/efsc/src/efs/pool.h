#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <shared_mutex>
#include <string>
#include <vector>

struct MappedFile {
    void*       base;
    std::size_t length;
    std::string path;
};

struct IndexEntry {
    int      file_index;
    uint64_t offset;
    uint64_t size;
};

struct FilePool {
    std::vector<MappedFile>           pool;
    std::map<std::string, IndexEntry> index;
    bool                              ready = false;
};

using PoolReadLock  = std::shared_lock<std::shared_mutex>;
using PoolWriteLock = std::unique_lock<std::shared_mutex>;
