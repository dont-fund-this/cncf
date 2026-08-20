#include "fit_help.h"

#include <string>

bool fit_help(const char* address) {
    return address && (std::string(address) == "sql.help" || std::string(address) == "help");
}
