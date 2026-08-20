#include "engine/db_close.h"

void db_close(sqlite3* db) {
    sqlite3_close(db);
}
