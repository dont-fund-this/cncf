#include "engine/parse/fit_parse.h"

#include <string>

namespace sql::engine {

bool fit_parse(const char* address) {
    return address && std::string(address) == "sql.parse";
}
}
