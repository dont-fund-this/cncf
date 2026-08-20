#include "engine/import/build_import_frames.h"
#include "engine/csv/line.h"
#include "engine/csv/load.h"
#include "engine/csv/map_open.h"
#include "engine/csv/map_close.h"
#include "engine/csv/tune.h"
#include "engine/db_open.h"
#include "engine/db_close.h"

#include <sqlite3.h>
#include <algorithm>
#include <string>

nlohmann::json build_import_frames(const std::string& target,
                                   const std::string& table,
                                   const std::string& file,
                                   const std::function<void(long)>& progress,
                                   const Flags& flags) {
    using nlohmann::json;
    json frames = json::array();

    Mapped src = map_open(file);
    if (!src.data) {
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open file: " + file}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }

    const char delim = flags.delimiter.empty() ? ',' : flags.delimiter[0];
    size_t pos = 0;
    const std::string_view first = line(src, pos);
    const size_t ncol = static_cast<size_t>(std::count(first.begin(), first.end(), delim)) + 1;
    if (!flags.header) pos = 0;

    sqlite3* db = db_open(target);
    if (!db) {
        map_close(src);
        frames.push_back({{"kind", "error"}, {"severity", "fatal"}, {"code", "open"}, {"text", "cannot open db: " + target}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }
    tune(db, flags);

    const Load r = load(db, table, src, pos, ncol, progress, flags);
    db_close(db);
    map_close(src);

    if (!r.prepared) {
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "prepare"}, {"text", "no such table: " + table}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }
    if (r.aborted) {
        frames.push_back({{"kind", "error"}, {"severity", "error"}, {"code", "row"}, {"text", "malformed row with skip disabled"}});
        frames.push_back({{"kind", "done"}, {"rc", 1}});
        return frames;
    }

    frames.push_back({{"kind", "info"}, {"text", "imported " + std::to_string(r.imported) + ", skipped " + std::to_string(r.skipped)}});
    frames.push_back({{"kind", "done"}, {"rc", 0}});
    return frames;
}
