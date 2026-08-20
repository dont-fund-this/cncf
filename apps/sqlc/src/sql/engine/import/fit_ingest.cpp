#include "engine/import/fit_ingest.h"

#include <string>

namespace sql::engine {

bool fit_ingest(const char* address) {
    return address && std::string(address) == "sql.ingest";
}
}
