#include "fit_selected_set.h"

#include <string>

namespace sql::database_object {

bool fit_selected_set(const char* address) {
    return address && std::string(address) == "sql.selected-object-set";
}

}
