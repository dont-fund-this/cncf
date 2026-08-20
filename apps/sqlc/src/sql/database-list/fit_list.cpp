#include "fit_list.h"

#include <string>

namespace sql::database_list {

bool fit_list(const char* address) {
    return address && std::string(address) == "sql.database-list";
}

}
