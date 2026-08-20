#include "fit_add.h"

#include <string>

namespace sql::database_list {

bool fit_add(const char* address) {
    return address && std::string(address) == "sql.database-add";
}

}
