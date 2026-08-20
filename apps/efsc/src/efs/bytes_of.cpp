#include "bytes.h"
#include "state.h"

BytesView bytes_of(const std::string& path) {
    BytesView v;
    auto& pool = efs::state().pool;

    auto it = pool.index.find(path);
    if (it == pool.index.end()) {
        v.error = "not found";
        return v;
    }

    const int idx = it->second.file_index;
    if (idx < 0 || idx >= static_cast<int>(pool.pool.size())) {
        v.error = "invalid pool index";
        return v;
    }

    v.data = static_cast<const char*>(pool.pool[idx].base) + it->second.offset;
    v.size = it->second.size;
    v.ok   = true;
    return v;
}
