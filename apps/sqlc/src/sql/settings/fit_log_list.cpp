#include "fit_log_list.h"

#include <string>

namespace sql::settings {

bool fit_log_list(const char* address) {
    return address && std::string(address) == "sql.log-list";
}

}
