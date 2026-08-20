#include "fit_get.h"

#include <string>

namespace sql::filter {

bool fit_get(const char* address) {
    return address && std::string(address) == "sql.filters-get";
}

}
