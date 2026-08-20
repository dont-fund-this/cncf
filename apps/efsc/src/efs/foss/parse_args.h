#pragma once

#include "args.h"
#include "parse_one_arg.h"

namespace foss {

inline int parse_args(int argc, char** argv, Args& out, int& bad_arg_index) {
    for (int i = 1; i < argc; i++) {
        if (parse_one_arg(argc, argv, i, out) != 0) {
            bad_arg_index = i;
            return 1;
        }
    }
    if (!out.output) return 2;
    return 0;
}

}
