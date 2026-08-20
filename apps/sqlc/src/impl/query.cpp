#include "../type.hpp"
#include <sqlite3.h>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
std::optional<std::string_view> json_find_field(std::string_view s, std::string_view key);
int register_csv(sqlite3* db);
extern "C" int Pump(Address address, Payload payload, Options options);

static std::string escape_json(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\b') out += "\\b";
        else if (c == '\f') out += "\\f";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (static_cast<unsigned char>(c) < 0x20) {
            char buf[8];
            std::snprintf(buf, sizeof(buf), "\\u%04x", c);
            out += buf;
        } else {
            out += c;
        }
    }
    return out;
}

extern const Def SqlQuery = {
    "sql.query",
    "sql.query",
    [](Address address, Payload, Options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return addr == "sql.query" || addr == "/sql/query";
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!SqlQuery.fit(address, payload, options)) return -1;

        std::string target = ":memory:";
        std::string sql_text;

        if (payload) {
            std::string_view p(payload);
            if (auto t = json_find_field(p, "target")) target = *t;
            if (auto s = json_find_field(p, "sql")) sql_text = *s;
            else sql_text = p;
        }

        sqlite3* db = nullptr;
        int rc = sqlite3_open_v2(target.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        if (rc != SQLITE_OK || !db) {
            std::string err = "{\"ok\":false,\"error\":\"failed to open database\"}";
            Address into = get_into(options);
            if (into) Pump(into, err.c_str(), "once");
            if (db) sqlite3_close(db);
            return 0;
        }

        register_csv(db);

        std::string frames_json = "[";
        const char* tail = sql_text.c_str();
        bool first_frame = true;
        int last_rc = SQLITE_OK;

        while (tail && *tail) {
            while (*tail && (*tail == ' ' || *tail == '\t' || *tail == '\n' || *tail == '\r' || *tail == ';')) tail++;
            if (!*tail) break;

            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db, tail, -1, &stmt, &tail);
            if (rc != SQLITE_OK) {
                last_rc = rc;
                const char* errMsg = sqlite3_errmsg(db);
                if (!first_frame) frames_json += ",";
                frames_json += "{\"kind\":\"error\",\"text\":\"" + escape_json(errMsg ? errMsg : "syntax error") + "\"}";
                first_frame = false;
                break;
            }

            int num_cols = sqlite3_column_count(stmt);
            if (num_cols > 0) {
                if (!first_frame) frames_json += ",";
                frames_json += "{\"kind\":\"resultset\",\"columns\":[";
                for (int c = 0; c < num_cols; ++c) {
                    if (c > 0) frames_json += ",";
                    frames_json += "\"" + escape_json(sqlite3_column_name(stmt, c)) + "\"";
                }
                frames_json += "]}";
                first_frame = false;

                std::string rows_json;
                bool first_row = true;
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    if (!first_row) rows_json += ",";
                    rows_json += "[";
                    for (int c = 0; c < num_cols; ++c) {
                        if (c > 0) rows_json += ",";
                        int col_type = sqlite3_column_type(stmt, c);
                        const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, c));
                        std::string t_name = "NULL";
                        if (col_type == SQLITE_INTEGER) t_name = "INTEGER";
                        else if (col_type == SQLITE_FLOAT) t_name = "FLOAT";
                        else if (col_type == SQLITE_TEXT) t_name = "TEXT";
                        else if (col_type == SQLITE_BLOB) t_name = "BLOB";

                        rows_json += "{\"t\":\"" + t_name + "\",\"v\":\"" + escape_json(val ? val : "") + "\"}";
                    }
                    rows_json += "]";
                    first_row = false;
                }

                if (!first_frame) frames_json += ",";
                frames_json += "{\"kind\":\"rows\",\"rows\":[" + rows_json + "]}";
            } else {
                rc = sqlite3_step(stmt);
            }

            sqlite3_finalize(stmt);
        }

        if (!first_frame) frames_json += ",";
        frames_json += "{\"kind\":\"done\",\"rc\":" + std::to_string(last_rc) + "}]";

        sqlite3_close(db);

        std::string final_payload = "{\"ok\":" + std::string(last_rc == SQLITE_OK ? "true" : "false") + ",\"frames\":" + frames_json + "}";
        Address into = get_into(options);
        if (into) {
            return Pump(into, final_payload.c_str(), "once");
        }
        return last_rc == SQLITE_OK ? 1 : 0;
    }
};
