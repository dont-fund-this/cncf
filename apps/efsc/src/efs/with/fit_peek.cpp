#include "fit_peek.h"

#include <string>

bool fit_peek(const char* address) {
    return address && std::string(address) == "efs.peek";
}
