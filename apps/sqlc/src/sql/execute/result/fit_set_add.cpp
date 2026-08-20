#include "fit_set_add.h"

#include <string>

namespace sql::execute::result {

bool fit_set_add(const char* address) {
    return address && std::string(address) == "sql.result-set-add";
}

}
