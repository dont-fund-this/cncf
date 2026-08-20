#include "csv.h"

int csv_close(sqlite3_vtab_cursor* cur) {
    delete reinterpret_cast<CsvCursor*>(cur);
    return SQLITE_OK;
}
