#include "fit_load.h"

#include <string>

bool fit_load(const char* address) {
    return address && std::string(address) == "efs.load";
}
