#include "foss/args.h"
#include "foss/parse_args.h"
#include "foss/append_entry.h"
#include "foss/write_central_record.h"
#include "foss/write_end_record.h"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace foss {
int build_zip(const char* output, int level, const std::vector<std::string>& files) {
    FILE* out = fopen(output, "wb");
    if (!out) return 1;

    std::vector<foss::Entry> entries;
    for (const auto& line : files) {
        if (line.empty()) continue;
        foss::append_entry(out, line, level, entries);
    }

    const uint32_t cd_off = (uint32_t)ftell(out);
    for (const auto& e : entries) foss::write_central_record(out, e);
    const uint32_t cd_size = (uint32_t)ftell(out) - cd_off;
    foss::write_end_record(out, entries.size(), cd_size, cd_off);

    fclose(out);
    return 0;
}
}
