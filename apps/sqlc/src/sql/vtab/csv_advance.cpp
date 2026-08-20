#include "csv.h"
#include "engine/csv/line.h"
#include "engine/csv/csv_parse.h"

void csv_advance(CsvCursor* cur) {
    CsvVtab* v = cur->vt;
    while (cur->pos < v->src.size) {
        const std::string_view ln = line(v->src, cur->pos);
        if (ln.empty()) continue;
        cur->owned.clear();
        csv_parse(ln, v->delim, cur->row, cur->owned);
        ++cur->rowid;
        return;
    }
    cur->eof = true;
}
