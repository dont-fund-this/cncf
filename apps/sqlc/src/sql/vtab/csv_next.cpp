#include "csv.h"

int csv_next(sqlite3_vtab_cursor* cur) {
    csv_advance(reinterpret_cast<CsvCursor*>(cur));
    return SQLITE_OK;
}
