#include "fit_log_add.h"

#include <string>

namespace sql::settings {

bool fit_log_add(const char* address) {
    return address && std::string(address) == "sql.log-add";
}

}
