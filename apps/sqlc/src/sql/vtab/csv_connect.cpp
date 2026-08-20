#include "csv.h"
#include "engine/csv/map_open.h"
#include "engine/csv/map_close.h"
#include "engine/csv/line.h"
#include "engine/csv/fields.h"
#include "engine/csv/header_ddl.h"
#include "engine/csv/col_affinities.h"

#include <string>
#include <vector>

int csv_connect(sqlite3* db, void*, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    CsvOpts o;
    csv_args(argc, argv, o);

    Mapped src = map_open(o.filename);
    if (!src.data) { *pzErr = sqlite3_mprintf("cannot open csv: %s", o.filename.c_str()); return SQLITE_ERROR; }

    size_t pos = 0;
    const std::string_view first = line(src, pos);
    std::vector<std::string_view> head;
    fields(first, head, o.delim);
    const int ncol = static_cast<int>(head.size());

    const std::string ddl = o.schema.empty() ? header_ddl(head, o.header) : o.schema;
    if (sqlite3_declare_vtab(db, ddl.c_str()) != SQLITE_OK) { map_close(src); return SQLITE_ERROR; }

    CsvVtab* vt = new CsvVtab();
    vt->src   = src;
    vt->body  = o.header ? pos : 0;
    vt->ncol  = ncol;
    vt->delim = o.delim;
    vt->aff   = o.schema.empty() ? std::vector<char>(ncol, 't') : col_affinities(o.schema);
    *ppVtab = &vt->base;
    return SQLITE_OK;
}
