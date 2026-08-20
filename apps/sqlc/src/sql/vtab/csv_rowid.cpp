#include "csv.h"

int csv_rowid(sqlite3_vtab_cursor* cur, sqlite3_int64* pRowid) {
    *pRowid = reinterpret_cast<CsvCursor*>(cur)->rowid;
    return SQLITE_OK;
}
