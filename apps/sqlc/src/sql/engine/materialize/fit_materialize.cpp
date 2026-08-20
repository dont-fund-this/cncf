#include "engine/materialize/fit_materialize.h"

#include <string>

namespace sql::engine {

bool fit_materialize(const char* address) {
    return address && std::string(address) == "sql.materialize";
}
}
