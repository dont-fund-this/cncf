#include "fit_selected_set.h"

#include <string>

namespace sql::execute::history {

bool fit_selected_set(const char* address) {
    return address && std::string(address) == "sql.selected-execute-set";
}

}
