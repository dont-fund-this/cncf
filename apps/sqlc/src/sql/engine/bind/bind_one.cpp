#include "engine/bind/bind_one.h"
#include "engine/csv/b64_decode.h"

#include <string>

void bind_one(sqlite3_stmt* st, int idx, const nlohmann::json& v) {
    if (v.is_null())           { sqlite3_bind_null(st, idx); return; }
    if (v.is_boolean())        { sqlite3_bind_int(st, idx, v.get<bool>() ? 1 : 0); return; }
    if (v.is_number_integer()) { sqlite3_bind_int64(st, idx, v.get<long long>()); return; }
    if (v.is_number_float())   { sqlite3_bind_double(st, idx, v.get<double>()); return; }
    if (v.is_object() && v.contains("blob") && v["blob"].is_string()) {
        const std::string bytes = b64_decode(v["blob"].get<std::string>());
        sqlite3_bind_blob(st, idx, bytes.data(), static_cast<int>(bytes.size()), SQLITE_TRANSIENT);
        return;
    }
    const std::string s = v.is_string() ? v.get<std::string>() : v.dump();
    sqlite3_bind_text(st, idx, s.c_str(), -1, SQLITE_TRANSIENT);
}
