#include "fit_add.h"

#include <string>

namespace sql::filter {

bool fit_add(const char* address) {
    return address && std::string(address) == "sql.filter-add";
}

}
