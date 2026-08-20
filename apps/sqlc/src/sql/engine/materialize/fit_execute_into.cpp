#include "engine/materialize/fit_execute_into.h"

#include <string>

namespace sql::engine {

bool fit_execute_into(const char* address) {
    return address && std::string(address) == "sql.execute-into";
}
}
