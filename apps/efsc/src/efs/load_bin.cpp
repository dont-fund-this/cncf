#include "bin.h"
#include "bin/internals.h"
#include "state.h"

#include <sys/mman.h>

bool load_bin(const std::string& path, std::string& err) {
    void*  map   = nullptr;
    size_t bytes = 0;
    if (!bin::open_mmap(path, map, bytes, err)) return false;

    auto unmap_fail = [&](const std::string& reason) {
        munmap(map, bytes);
        err = reason + ": " + path;
        return false;
    };

    const uint8_t* base = static_cast<const uint8_t*>(map);
    uint32_t count = 0;
    std::string parse_err;
    if (!bin::parse_header(base, bytes, count, parse_err)) return unmap_fail(parse_err);

    std::vector<ParsedEntry> entries;
    if (!bin::parse_entries(base, bytes, count, entries, parse_err)) return unmap_fail(parse_err);

    auto& pool = efs::state().pool;
    const int mapping_idx = static_cast<int>(pool.pool.size());
    pool.pool.push_back({map, bytes, path});
    for (auto& e : entries) {
        pool.index[e.path] = {mapping_idx, e.offset, e.size};
    }
    pool.ready = true;
    return true;
}
