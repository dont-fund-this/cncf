#include "csv.h"

int csv_open(sqlite3_vtab* vt, sqlite3_vtab_cursor** ppCur) {
    CsvCursor* cur = new CsvCursor();
    cur->vt = reinterpret_cast<CsvVtab*>(vt);
    *ppCur = &cur->base;
    return SQLITE_OK;
}
