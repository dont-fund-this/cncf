#include "engine/import/fit_import_real_flags.h"

#include <string>

namespace sql::engine {

bool fit_import_real_flags(const char* address) {
    return address && std::string(address) == "sql.import.real.flags";
}
}
