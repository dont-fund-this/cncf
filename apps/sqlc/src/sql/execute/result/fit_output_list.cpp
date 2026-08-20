#include "fit_output_list.h"

#include <string>

namespace sql::execute::result {

bool fit_output_list(const char* address) {
    return address && std::string(address) == "sql.output-list";
}

}
