#include "fit_selected_execute.h"

#include <string>

namespace sql::text {

bool fit_selected_execute(const char* address) {
    return address && std::string(address) == "sql.selected-text-execute";
}

}
