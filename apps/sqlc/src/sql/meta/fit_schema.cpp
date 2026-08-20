#include "fit_schema.h"

#include <string>

namespace sql::meta {

bool fit_schema(const char* address) {
    return address && std::string(address) == "sql.schema";
}

}
