#include "fit_clear.h"

#include <string>

namespace sql::filter {

bool fit_clear(const char* address) {
    return address && std::string(address) == "sql.filters-clear";
}

}
