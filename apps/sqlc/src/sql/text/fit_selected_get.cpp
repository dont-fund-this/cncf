#include "fit_selected_get.h"

#include <string>

namespace sql::text {

bool fit_selected_get(const char* address) {
    return address && std::string(address) == "sql.selected-text-get";
}

}
