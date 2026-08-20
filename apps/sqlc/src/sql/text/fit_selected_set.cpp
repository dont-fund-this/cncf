#include "fit_selected_set.h"

#include <string>

namespace sql::text {

bool fit_selected_set(const char* address) {
    return address && std::string(address) == "sql.selected-text-set";
}

}
