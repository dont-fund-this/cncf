#include "fit_save.h"

#include <string>

namespace sql::settings {

bool fit_save(const char* address) {
    return address && std::string(address) == "sql.settings-save";
}

}
