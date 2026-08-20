#include "engine/materialize/materialize_each.h"
#include "engine/parse/statements.h"
#include "engine/materialize/produces_rows.h"
#include "engine/exec.h"
#include "engine/has_error.h"
#include "engine/quote_ident.h"
#include "engine/squote.h"

#include <nlohmann/json.hpp>

std::vector<std::string> materialize_each(sqlite3* db, const std::string& prefix, long execute_id, const std::string& sql) {
    using nlohmann::json;
    std::vector<std::string> names;
    int n = 0;
    for (const auto& stmt : statements(sql)) {
        if (!produces_rows(db, stmt)) { exec(db, stmt); continue; }
        const std::string name = "result_" + std::to_string(execute_id) + "_" + std::to_string(n);
        const json f = exec(db, "CREATE TABLE " + prefix + quote_ident(name) + " AS " + stmt);
        if (has_error(f)) continue;
        exec(db, "INSERT INTO " + prefix + "ResultInfo(ExecuteId, ResultName) VALUES (" + std::to_string(execute_id) + ",'" + squote(name) + "')");
        names.push_back(name);
        n++;
    }
    return names;
}
