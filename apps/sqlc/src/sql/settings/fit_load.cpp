#include "fit_load.h"

#include <string>

namespace sql::settings {

bool fit_load(const char* address) {
    return address && std::string(address) == "sql.settings-load";
}

}
