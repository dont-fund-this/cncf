#include "fit_indexes.h"

#include <string>

namespace sql::meta {

bool fit_indexes(const char* address) {
    return address && std::string(address) == "sql.indexes";
}

}
