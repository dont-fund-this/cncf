#include "engine/import/fit_import_virt_flags.h"

#include <string>

namespace sql::engine {

bool fit_import_virt_flags(const char* address) {
    return address && std::string(address) == "sql.import.virt.flags";
}
}
