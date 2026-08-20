#include "fit_copy.h"

#include <string>

namespace sql::database {

bool fit_copy(const char* address) {
    return address && std::string(address) == "sql.database-copy";
}

}
