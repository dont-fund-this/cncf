#include "fit_option_set.h"

#include <string>

namespace sql::settings {

bool fit_option_set(const char* address) {
    return address && std::string(address) == "sql.option-set";
}

}
