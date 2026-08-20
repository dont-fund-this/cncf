#include "engine/fit_query.h"

#include <string>

namespace sql::engine {

bool fit_query(const char* address) {
    return address && std::string(address) == "sql.query";
}
}
