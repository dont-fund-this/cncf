#include "csv.h"

int csv_bestindex(sqlite3_vtab*, sqlite3_index_info* info) {
    info->estimatedCost = 1000000.0;
    return SQLITE_OK;
}
