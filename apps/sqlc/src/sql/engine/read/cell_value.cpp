#include "engine/read/cell_value.h"
#include "engine/csv/b64_encode.h"

#include <string>

nlohmann::json cell_value(sqlite3_stmt* st, int i, int type) {
    if (type == SQLITE_NULL) return nullptr;
    if (type == SQLITE_BLOB) {
        const unsigned char* b = static_cast<const unsigned char*>(sqlite3_column_blob(st, i));
        return b64_encode(b, static_cast<std::size_t>(sqlite3_column_bytes(st, i)));
    }
    const unsigned char* x = sqlite3_column_text(st, i);
    return x ? nlohmann::json(std::string(reinterpret_cast<const char*>(x))) : nlohmann::json(nullptr);
}
