#include "fit_foreign_keys.h"

#include <string>

namespace sql::meta {

bool fit_foreign_keys(const char* address) {
    return address && std::string(address) == "sql.foreign-keys";
}

}
