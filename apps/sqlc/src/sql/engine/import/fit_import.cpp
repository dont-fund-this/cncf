#include "engine/import/fit_import.h"

#include <string>

namespace sql::engine {

bool fit_import(const char* address) {
    return address && std::string(address) == "sql.import";
}
}
