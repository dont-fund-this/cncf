#include "csv.h"

int csv_eof(sqlite3_vtab_cursor* cur) {
    return reinterpret_cast<CsvCursor*>(cur)->eof ? 1 : 0;
}
