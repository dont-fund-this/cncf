#include "fit_list.h"

#include <string>

bool fit_list(const char* address) {
    return address && std::string(address) == "efs.list";
}
