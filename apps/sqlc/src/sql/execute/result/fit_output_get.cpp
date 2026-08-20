#include "fit_output_get.h"

#include <string>

namespace sql::execute::result {

bool fit_output_get(const char* address) {
    return address && std::string(address) == "sql.output-get";
}

}
