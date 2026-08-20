#include "csv.h"
#include "engine/csv/map_close.h"

int csv_disconnect(sqlite3_vtab* vt) {
    CsvVtab* v = reinterpret_cast<CsvVtab*>(vt);
    map_close(v->src);
    delete v;
    return SQLITE_OK;
}
