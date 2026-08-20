#include "fit_read.h"

#include <string>

bool fit_read(const char* address) {
    return address && std::string(address) == "efs.read";
}
