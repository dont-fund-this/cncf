#include "fit_columns.h"

#include <string>

namespace sql::meta {

bool fit_columns(const char* address) {
    return address && std::string(address) == "sql.columns";
}

}
