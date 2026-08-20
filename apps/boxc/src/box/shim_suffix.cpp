#include "state.h"

#include <cstdio>

namespace box {

std::string shim_suffix(const std::string& shim) {
    if (shim.empty()) return {};
    char buf[80];
    std::snprintf(buf, sizeof(buf), " PAT_SHIM=0x%llx PAT_DOORBELL=0x%llx",
                  static_cast<unsigned long long>(SHIM_ADDR),
                  static_cast<unsigned long long>(DOORBELL_ADDR));
    return buf;
}

}
