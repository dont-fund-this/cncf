#include "fit_export.h"

#include <string>

bool fit_export(const char* address) {
    return address && std::string(address) == "efs.export";
}
