#include "engine/fit_explain.h"

#include <string>

namespace sql::engine {

bool fit_explain(const char* address) {
    return address && std::string(address) == "sql.explain";
}
}
