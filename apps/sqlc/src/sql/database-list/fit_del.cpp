#include "fit_del.h"

#include <string>

namespace sql::database_list {

bool fit_del(const char* address) {
    return address && std::string(address) == "sql.database-del";
}

}
