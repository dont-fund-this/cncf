#include "engine/csv/load.h"
#include "engine/csv/line.h"
#include "engine/csv/csv_parse.h"
#include "engine/csv/feed.h"
#include "engine/csv/insert.h"
#include "engine/quote_ident.h"

#include <deque>
#include <string_view>
#include <vector>

Load load(sqlite3* db, const std::string& table, const Mapped& src, size_t pos, size_t ncol,
          const std::function<void(long)>& progress, const Flags& flags) {
    const size_t batch_rows = flags.batch_rows > 0 ? static_cast<size_t>(flags.batch_rows) : 1;
    const long commit_every = flags.commit_every;
    const char delim = flags.delimiter.empty() ? ',' : flags.delimiter[0];

    Load r;
    sqlite3_stmt* full = nullptr;
    if (sqlite3_prepare_v2(db, insert(table, ncol, batch_rows).c_str(), -1, &full, nullptr) != SQLITE_OK) {
        r.prepared = false;
        return r;
    }
    if (flags.replace) sqlite3_exec(db, ("DELETE FROM " + quote_ident(table)).c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);

    std::vector<std::string_view> row;   row.reserve(ncol);
    std::vector<std::string_view> batch; batch.reserve(batch_rows * ncol);
    std::deque<std::string> owned;
    long rows = 0, since = 0;
    while (pos < src.size) {
        const std::string_view ln = line(src, pos);
        if (ln.empty()) continue;
        csv_parse(ln, delim, row, owned);
        if (row.size() != ncol && flags.skip_bad) { ++r.skipped; continue; }
        if (row.size() != ncol) { r.aborted = true; break; }
        batch.insert(batch.end(), row.begin(), row.end());
        if (++rows < static_cast<long>(batch_rows)) continue;
        feed(full, batch);
        r.imported += rows; since += rows; rows = 0; batch.clear(); owned.clear();
        if (progress) progress(r.imported);
        if (since >= commit_every) { sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr); sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr); since = 0; }
    }
    if (!r.aborted && rows > 0) {
        sqlite3_stmt* rem = nullptr;
        sqlite3_prepare_v2(db, insert(table, ncol, static_cast<size_t>(rows)).c_str(), -1, &rem, nullptr);
        feed(rem, batch);
        sqlite3_finalize(rem);
        r.imported += rows;
    }
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_finalize(full);
    return r;
}
