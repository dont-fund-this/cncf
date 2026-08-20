#include "fit_list_get.h"

#include <string>

namespace sql::execute::result {

bool fit_list_get(const char* address) {
    return address && std::string(address) == "sql.result-list-get";
}

}
