#include "engine/csv/tune.h"

#include <string>

void tune(sqlite3* db, const Flags& flags) {
    const std::string sql =
        "PRAGMA synchronous="  + flags.synchronous  + ";"
        "PRAGMA journal_mode=" + flags.journal_mode + ";"
        "PRAGMA cache_size="   + std::to_string(flags.cache_size) + ";"
        "PRAGMA temp_store="   + flags.temp_store   + ";"
        "PRAGMA locking_mode=" + flags.locking_mode + ";"
        "PRAGMA mmap_size="    + std::to_string(flags.mmap_size) + ";";
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}
