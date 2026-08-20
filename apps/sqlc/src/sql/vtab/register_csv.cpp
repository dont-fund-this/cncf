#include "csv.h"

static sqlite3_module csv_mod = {
    .iVersion    = 0,
    .xCreate     = csv_connect,
    .xConnect    = csv_connect,
    .xBestIndex  = csv_bestindex,
    .xDisconnect = csv_disconnect,
    .xDestroy    = csv_disconnect,
    .xOpen       = csv_open,
    .xClose      = csv_close,
    .xFilter     = csv_filter,
    .xNext       = csv_next,
    .xEof        = csv_eof,
    .xColumn     = csv_column,
    .xRowid      = csv_rowid,
};

int register_csv(sqlite3* db) {
    return sqlite3_create_module(db, "csv", &csv_mod, nullptr);
}
