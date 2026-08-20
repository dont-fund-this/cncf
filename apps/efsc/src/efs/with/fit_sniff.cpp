#include "fit_sniff.h"

#include <string>

bool fit_sniff(const char* address) {
    return address && std::string(address) == "efs.sniff";
}
