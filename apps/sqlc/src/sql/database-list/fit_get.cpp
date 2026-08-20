#include "fit_get.h"

#include <string>

namespace sql::database_list {

bool fit_get(const char* address) {
    return address && std::string(address) == "sql.database-get";
}

}
