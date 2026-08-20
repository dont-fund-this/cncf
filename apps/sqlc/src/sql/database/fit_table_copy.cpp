#include "fit_table_copy.h"

#include <string>

namespace sql::database {

bool fit_table_copy(const char* address) {
    return address && std::string(address) == "sql.table-copy";
}

}
