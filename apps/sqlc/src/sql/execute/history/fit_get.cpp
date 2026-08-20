#include "fit_get.h"

#include <string>

namespace sql::execute::history {

bool fit_get(const char* address) {
    return address && std::string(address) == "sql.execute-history-get";
}

}
