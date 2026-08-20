#include "engine/csv/feed.h"

bool feed(sqlite3_stmt* st, const std::vector<std::string_view>& vals) {
    for (size_t i = 0; i < vals.size(); ++i)
        sqlite3_bind_text(st, static_cast<int>(i) + 1, vals[i].data(),
                          static_cast<int>(vals[i].size()), SQLITE_STATIC);
    const bool ok = sqlite3_step(st) == SQLITE_DONE;
    sqlite3_reset(st);
    return ok;
}
