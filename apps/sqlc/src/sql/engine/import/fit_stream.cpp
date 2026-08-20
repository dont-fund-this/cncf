#include "engine/import/fit_stream.h"

#include <string>

namespace sql::engine {

bool fit_stream(const char* address) {
    return address && std::string(address) == "sql.stream";
}
}
