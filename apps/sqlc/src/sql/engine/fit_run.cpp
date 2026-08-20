#include "engine/fit_run.h"

#include <string>

namespace sql::engine {

bool fit_run(const char* address) {
    return address && std::string(address) == "sql.run";
}
}
