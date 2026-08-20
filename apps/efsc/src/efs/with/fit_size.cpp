#include "fit_size.h"

#include <string>

bool fit_size(const char* address) {
    return address && std::string(address) == "efs.size";
}
