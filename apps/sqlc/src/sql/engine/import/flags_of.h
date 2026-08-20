#pragma once

#include "engine/import/flags.h"
#include <nlohmann/json.hpp>

inline Flags flags_of(const nlohmann::json& p) {
    Flags f;
    if (!p.is_object()) return f;
    f.synchronous  = p.value("synchronous",  f.synchronous);
    f.journal_mode = p.value("journal_mode", f.journal_mode);
    f.cache_size   = p.value("cache_size",   f.cache_size);
    f.temp_store   = p.value("temp_store",   f.temp_store);
    f.locking_mode = p.value("locking_mode", f.locking_mode);
    f.mmap_size    = p.value("mmap_size",    f.mmap_size);
    f.header       = p.value("header",       f.header);
    f.delimiter    = p.value("delimiter",    f.delimiter);
    f.batch_rows   = p.value("batch",        f.batch_rows);
    f.commit_every = p.value("commit",       f.commit_every);
    f.replace      = p.value("replace",      f.replace);
    f.skip_bad     = p.value("skip",         f.skip_bad);
    return f;
}
