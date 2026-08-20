#include "csv.h"

int csv_filter(sqlite3_vtab_cursor* cur, int, const char*, int, sqlite3_value**) {
    CsvCursor* c = reinterpret_cast<CsvCursor*>(cur);
    c->pos = c->vt->body;
    c->rowid = 0;
    c->eof = false;
    csv_advance(c);
    return SQLITE_OK;
}
