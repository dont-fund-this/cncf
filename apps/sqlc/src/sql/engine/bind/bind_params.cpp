#include "engine/bind/bind_params.h"
#include "engine/bind/bind_one.h"

#include <string>

void bind_params(sqlite3_stmt* st, const nlohmann::json& params) {
    if (params.is_array()) {
        int i = 1;
        for (const auto& v : params) bind_one(st, i++, v);
        return;
    }
    if (!params.is_object()) return;
    for (auto it = params.begin(); it != params.end(); ++it) {
        const int idx = sqlite3_bind_parameter_index(st, (":" + it.key()).c_str());
        if (idx > 0) bind_one(st, idx, it.value());
    }
}
