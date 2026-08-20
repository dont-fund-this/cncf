#include "fit_output_record.h"

#include <string>

namespace sql::execute::result {

bool fit_output_record(const char* address) {
    return address && std::string(address) == "sql.output-record";
}

}
