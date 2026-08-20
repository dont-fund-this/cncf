#pragma once

#include "args.h"
#include "is_level_flag.h"

#include <cstring>

namespace foss {

inline int parse_one_arg(int argc, char** argv, int& i, Args& out) {
    const char* a = argv[i];
    if (is_level_flag(a)) { out.level = a[1] - '0'; return 0; }
    if (strcmp(a, "-o") == 0 && i + 1 < argc) { out.output = argv[++i]; return 0; }
    return 1;
}

}
