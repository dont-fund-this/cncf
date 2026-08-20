#include "fit_selected_get.h"

#include <string>

namespace sql::execute::history {

bool fit_selected_get(const char* address) {
    return address && std::string(address) == "sql.selected-execute-get";
}

}
