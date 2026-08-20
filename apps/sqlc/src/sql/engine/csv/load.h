#pragma once

#include "engine/csv/mapped.h"
#include "engine/import/flags.h"
#include <sqlite3.h>
#include <cstddef>
#include <functional>
#include <string>

struct Load {
    long imported = 0;
    long skipped  = 0;
    bool prepared = true;
    bool aborted  = false;
};

Load load(sqlite3* db, const std::string& table, const Mapped& src, size_t pos, size_t ncol,
          const std::function<void(long)>& progress = {}, const Flags& flags = {});
