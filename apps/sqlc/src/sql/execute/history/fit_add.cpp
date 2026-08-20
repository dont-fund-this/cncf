#include "fit_add.h"

#include <string>

namespace sql::execute::history {

bool fit_add(const char* address) {
    return address && std::string(address) == "sql.execute-history-add";
}

}
