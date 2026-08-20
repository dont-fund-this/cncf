#include "../type.hpp"
#include <sqlite3.h>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

Address get_into(Options options);
std::optional<std::string_view> json_find_field(std::string_view s, std::string_view key);
extern "C" int Pump(Address address, Payload payload, Options options);

static std::string escape_json(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 4);
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

extern const Def SqlSchema = {
    "sql.schema",
    "sql.schema",
    [](Address address, Payload, Options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return addr == "sql.schema" || addr == "/sql/schema";
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!SqlSchema.fit(address, payload, options)) return -1;

        std::string target = ":memory:";
        if (payload) {
            std::string_view p(payload);
            if (auto t = json_find_field(p, "target")) target = *t;
        }

        sqlite3* db = nullptr;
        int rc = sqlite3_open_v2(target.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
        if (rc != SQLITE_OK || !db) {
            std::string err = "{\"ok\":false,\"tables\":[]}";
            Address into = get_into(options);
            if (into) Pump(into, err.c_str(), "once");
            if (db) sqlite3_close(db);
            return 0;
        }

        sqlite3_stmt* stmt = nullptr;
        const char* q = "SELECT type, name, tbl_name, sql FROM sqlite_master WHERE name NOT LIKE 'sqlite_%' ORDER BY name";
        rc = sqlite3_prepare_v2(db, q, -1, &stmt, nullptr);
        std::string tables_json = "[";
        bool first = true;

        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                if (!first) tables_json += ",";
                const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* sql = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

                tables_json += "{\"type\":\"" + escape_json(type ? type : "") +
                               "\",\"name\":\"" + escape_json(name ? name : "") +
                               "\",\"sql\":\"" + escape_json(sql ? sql : "") + "\"}";
                first = false;
            }
            sqlite3_finalize(stmt);
        }

        tables_json += "]";
        sqlite3_close(db);

        std::string res = "{\"ok\":true,\"schema\":" + tables_json + "}";
        Address into = get_into(options);
        if (into) {
            return Pump(into, res.c_str(), "once");
        }
        return 1;
    }
};

extern const Def SqlColumns = {
    "sql.columns",
    "sql.columns",
    [](Address address, Payload, Options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return addr == "sql.columns" || addr == "/sql/columns";
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!SqlColumns.fit(address, payload, options)) return -1;

        std::string target = ":memory:";
        std::string table;
        if (payload) {
            std::string_view p(payload);
            if (auto t = json_find_field(p, "target")) target = *t;
            if (auto tbl = json_find_field(p, "table")) table = *tbl;
        }

        if (table.empty()) return 0;

        sqlite3* db = nullptr;
        int rc = sqlite3_open_v2(target.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
        if (rc != SQLITE_OK || !db) {
            std::string err = "{\"ok\":false,\"columns\":[]}";
            Address into = get_into(options);
            if (into) Pump(into, err.c_str(), "once");
            if (db) sqlite3_close(db);
            return 0;
        }

        std::string q = "PRAGMA table_info(" + table + ")";
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr);
        std::string cols_json = "[";
        bool first = true;

        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                if (!first) cols_json += ",";
                const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                int notnull = sqlite3_column_int(stmt, 3);
                int pk = sqlite3_column_int(stmt, 5);

                cols_json += "{\"name\":\"" + escape_json(name ? name : "") +
                             "\",\"type\":\"" + escape_json(type ? type : "") +
                             "\",\"notnull\":" + (notnull ? "true" : "false") +
                             ",\"pk\":" + (pk ? "true" : "false") + "}";
                first = false;
            }
            sqlite3_finalize(stmt);
        }

        cols_json += "]";
        sqlite3_close(db);

        std::string res = "{\"ok\":true,\"table\":\"" + escape_json(table) + "\",\"columns\":" + cols_json + "}";
        Address into = get_into(options);
        if (into) {
            return Pump(into, res.c_str(), "once");
        }
        return 1;
    }
};
