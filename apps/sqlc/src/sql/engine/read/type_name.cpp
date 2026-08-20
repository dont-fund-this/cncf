#include "engine/read/type_name.h"

const char* type_name(int sqlite_type) {
    static const char* const names[] = {"", "INTEGER", "REAL", "TEXT", "BLOB", "NULL"};
    return (sqlite_type >= 1 && sqlite_type <= 5) ? names[sqlite_type] : "";
}
