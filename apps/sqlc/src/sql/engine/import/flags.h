#pragma once

#include <string>

struct Flags {
    std::string synchronous  = "OFF";
    std::string journal_mode = "OFF";
    long        cache_size   = 20000;
    std::string temp_store   = "MEMORY";
    std::string locking_mode = "EXCLUSIVE";
    long        mmap_size    = 268435456;
    bool        header       = true;
    std::string delimiter    = ",";
    long        batch_rows   = 1000;
    long        commit_every = 500000;
    bool        replace      = true;
    bool        skip_bad     = true;
};
