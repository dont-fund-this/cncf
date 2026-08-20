#include "fit_page.h"

#include <string>

namespace sql::execute::result {

bool fit_page(const char* address) {
    return address && std::string(address) == "sql.result-page";
}

}
