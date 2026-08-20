#include "engine/bind/fit_bind.h"

#include <string>

namespace sql::engine {

bool fit_bind(const char* address) {
    return address && std::string(address) == "sql.bind";
}
}
