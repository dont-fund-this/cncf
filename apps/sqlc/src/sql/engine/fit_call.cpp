#include "engine/fit_call.h"

#include <string>

namespace sql::engine {

bool fit_call(const char* address) {
    return address && std::string(address) == "sql.call";
}
}
