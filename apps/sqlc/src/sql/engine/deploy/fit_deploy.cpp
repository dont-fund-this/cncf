#include "engine/deploy/fit_deploy.h"

#include <string>

namespace sql::engine {

bool fit_deploy(const char* address) {
    return address && std::string(address) == "sql.deploy";
}
}
