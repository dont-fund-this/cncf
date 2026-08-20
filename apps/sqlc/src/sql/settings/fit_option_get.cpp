#include "fit_option_get.h"

#include <string>

namespace sql::settings {

bool fit_option_get(const char* address) {
    return address && std::string(address) == "sql.option-get";
}

}
