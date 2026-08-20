#include "engine/import/fit_import_virtual.h"

#include <string>

namespace sql::engine {

bool fit_import_virtual(const char* address) {
    return address && std::string(address) == "sql.import.virtual";
}
}
